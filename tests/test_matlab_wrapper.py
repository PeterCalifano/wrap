"""
Unit tests for Matlab wrap program
Author: Matthew Sklar, Varun Agrawal
Date: March 2019
"""
# pylint: disable=import-error, wrong-import-position

from gtwrap.matlab_wrapper import MatlabWrapper
from gtwrap.matlab_wrapper import wrapper as matlab_wrapper_module
import filecmp
import os
import os.path as osp
import sys
import tempfile
import unittest
from unittest import mock

sys.path.append(osp.dirname(osp.dirname(osp.abspath(__file__))))

from gtwrap.matlab_wrapper import wrapper as matlab_wrapper_module
from gtwrap.matlab_wrapper import MatlabWrapper, MatlabWrapperCpp


class TestWrap(unittest.TestCase):
    """
    Test the Matlab wrapper
    """

    def setUp(self) -> None:
        super().setUp()

        # Set up all the directories
        self.TEST_DIR = osp.dirname(osp.realpath(__file__))
        self.INTERFACE_DIR = osp.join(self.TEST_DIR, "fixtures")
        self.MATLAB_TEST_DIR = osp.join(self.TEST_DIR, "expected", "matlab")
        self.MATLAB_ACTUAL_DIR = osp.join(self.TEST_DIR, "actual", "matlab")

        if not osp.exists(self.MATLAB_ACTUAL_DIR):
            os.mkdir(self.MATLAB_ACTUAL_DIR)

        # Generate the matlab.h file if it does not exist
        template_file = osp.join(self.TEST_DIR, "..", "gtwrap",
                                 "matlab_wrapper", "matlab_wrapper.tpl")
        if not osp.exists(template_file):
            with open(template_file, 'w', encoding="UTF-8") as tpl:
                tpl.write("#include <wrap/matlab.h>\n#include <map>\n")

        # Create the `actual/matlab` directory
        os.makedirs(self.MATLAB_ACTUAL_DIR, exist_ok=True)

    def compare_and_diff(self, file, actual):
        """
        Compute the comparison between the expected and actual file,
        and assert if diff is zero.
        """
        expected = osp.join(self.MATLAB_TEST_DIR, file)
        success = filecmp.cmp(actual, expected)

        if not success:
            os.system(f"diff {actual} {expected}")
        self.assertTrue(success, f"Mismatch for file {file}")

    def _make_fallback_template_root(self, root_name: str, create_matlab_header: bool) -> str:
        wrap_root = osp.join(self.MATLAB_ACTUAL_DIR, root_name)
        os.makedirs(osp.join(wrap_root, "gtwrap",
                    "matlab_wrapper"), exist_ok=True)
        os.makedirs(osp.join(wrap_root, "templates"), exist_ok=True)

        template_path = osp.join(
            wrap_root, "templates", "matlab_wrapper.tpl.in")
        with open(template_path, "w", encoding="UTF-8") as tpl:
            tpl.write(
                "#include <${GTWRAP_INCLUDE_NAME}/matlab.h>\n#include <map>\n")

        if create_matlab_header:
            with open(osp.join(wrap_root, "matlab.h"), "w", encoding="UTF-8") as matlab_h:
                matlab_h.write("// test header\n")

        wrapper_path = osp.join(wrap_root, "gtwrap",
                                "matlab_wrapper", "wrapper.py")
        with open(wrapper_path, "w", encoding="UTF-8") as wrapper_file:
            wrapper_file.write("# test wrapper path\n")

        return wrapper_path

    def test_template_loader_falls_back_to_tpl_in(self):
        with tempfile.TemporaryDirectory(dir=self.MATLAB_ACTUAL_DIR) as temp_dir:
            install_wrapper_path = self._make_fallback_template_root(
                osp.join(osp.basename(temp_dir), "gtwrap_install"), False)
            local_wrapper_path = self._make_fallback_template_root(
                osp.join(osp.basename(temp_dir), "wrap"), True)

            with mock.patch.object(matlab_wrapper_module, "__file__", install_wrapper_path):
                install_headers = MatlabWrapper._load_wrapper_file_headers()
            self.assertEqual(install_headers.splitlines()[
                             0], "#include <gtwrap/matlab.h>")

            with mock.patch.object(matlab_wrapper_module, "__file__", local_wrapper_path):
                local_headers = MatlabWrapper._load_wrapper_file_headers()
            self.assertEqual(local_headers.splitlines()[
                             0], "#include <wrap/matlab.h>")

    def test_matrix_view_arguments(self):
        """Test that matrix view arguments use MATLAB double arrays directly."""
        file = osp.join(self.INTERFACE_DIR, 'matrix_views.i')

        wrapper = MatlabWrapper(module_name='matrix_views',
                                top_module_namespace=['gtsam'],
                                ignore_classes=[''])

        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        cpp_file = osp.join(self.MATLAB_ACTUAL_DIR, 'matrix_views_wrapper.cpp')
        with open(cpp_file, 'r', encoding='UTF-8') as f:
            cpp_content = f.read()

        self.assertIn(
            'gtsam::ConstMatrixView points = unwrapMatrixView< gtsam::ConstMatrixView >(in[1]);',
            cpp_content)
        self.assertIn('obj->acceptView(points);', cpp_content)
        self.assertIn('obj->scaleView(points,scale)', cpp_content)
        self.assertNotIn('unwrap< gtsam::ConstMatrixView >', cpp_content)
        self.assertNotIn('*points', cpp_content)

        m_file = osp.join(self.MATLAB_ACTUAL_DIR, '+gtsam',
                          'MatrixViewFixture.m')
        with open(m_file, 'r', encoding='UTF-8') as f:
            matlab_content = f.read()

        self.assertIn("isa(varargin{1},'double')", matlab_content)

        matlab_header = osp.join(self.TEST_DIR, '..', 'matlab.h')
        with open(matlab_header, 'r', encoding='UTF-8') as f:
            header_content = f.read()

        self.assertIn('unwrapMatrixView', header_content)
        self.assertIn('mxIsSparse(array)', header_content)
        self.assertIn('mwSize rows', header_content)
        self.assertIn('static_cast<unsigned long long>(rows)', header_content)
        self.assertIn('Eigen::Index m', header_content)
        self.assertIn('Stride(m, 1)', header_content)

    def test_geometry(self):
        """
        Check generation of matlab geometry wrapper.
        python3 wrap/matlab_wrapper.py --src wrap/tests/geometry.h
            --module_name geometry --out wrap/tests/actual-matlab
        """
        file = osp.join(self.INTERFACE_DIR, 'geometry.i')

        # Create MATLAB wrapper instance
        wrapper = MatlabWrapper(module_name='geometry',
                                top_module_namespace=['gtsam'],
                                ignore_classes=[''],
                                use_boost_serialization=True)

        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        files = ['+gtsam/Point2.m', '+gtsam/Point3.m', 'geometry_wrapper.cpp']

        self.assertTrue(osp.isdir(osp.join(self.MATLAB_ACTUAL_DIR, '+gtsam')))

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_functions(self):
        """Test interface file with function info."""
        file = osp.join(self.INTERFACE_DIR, 'functions.i')

        wrapper = MatlabWrapper(
            module_name='functions',
            top_module_namespace=['gtsam'],
            ignore_classes=[''],
        )

        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        files = [
            'functions_wrapper.cpp',
            'aGlobalFunction.m',
            'load2D.m',
            'MultiTemplatedFunctionDoubleSize_tDouble.m',
            'MultiTemplatedFunctionStringSize_tDouble.m',
            'overloadedGlobalFunction.m',
            'TemplatedFunctionRot3.m',
            'DefaultFuncInt.m',
            'DefaultFuncObj.m',
            'DefaultFuncString.m',
            'DefaultFuncVector.m',
            'DefaultFuncZero.m',
            'setPose.m',
            'EliminateDiscrete.m',
            'triangulatePoint3Cal3_S2.m',
            'FindKarcherMeanPoint3.m',
            'FindKarcherMeanSO3.m',
            'FindKarcherMeanSO4.m',
            'FindKarcherMeanPose3.m',
        ]

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_class(self):
        """Test interface file with only class info."""
        file = osp.join(self.INTERFACE_DIR, 'class.i')

        wrapper = MatlabWrapper(
            module_name='class',
            top_module_namespace=['gtsam'],
            ignore_classes=[''],
        )

        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        files = [
            'class_wrapper.cpp',
            'ForwardKinematics.m',
            'FunDouble.m',
            'FunRange.m',
            'HessianFactor.m',
            'MultipleTemplatesIntDouble.m',
            'MultipleTemplatesIntFloat.m',
            'MyFactorPosePoint2.m',
            'MyVector3.m',
            'MyVector12.m',
            'PrimitiveRefDouble.m',
            'SmartProjectionRigFactorPinholeCameraCal3_S2.m',
            'Test.m',
        ]

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_enum(self):
        """Test interface file with only enum info."""
        file = osp.join(self.INTERFACE_DIR, 'enum.i')

        wrapper = MatlabWrapper(
            module_name='enum',
            top_module_namespace=['gtsam'],
            ignore_classes=[''],
        )

        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        files = [
            'enum_wrapper.cpp',
            'Color.m',
            'Pet.m',
            '+Pet/Kind.m',
            '+gtsam/VerbosityLM.m',
            '+gtsam/+MCU/Avengers.m',
            '+gtsam/+MCU/GotG.m',
            '+gtsam/+OptimizerGaussNewtonParams/Verbosity.m',
        ]

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_templates(self):
        """Test interface file with template info."""
        file = osp.join(self.INTERFACE_DIR, 'templates.i')

        wrapper = MatlabWrapper(
            module_name='template',
            top_module_namespace=['gtsam'],
            ignore_classes=[''],
        )

        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        files = [
            'template_wrapper.cpp', 'ScopedTemplateResult.m',
            'TemplatedConstructor.m'
        ]

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_inheritance(self):
        """Test interface file with class inheritance definitions."""
        file = osp.join(self.INTERFACE_DIR, 'inheritance.i')

        wrapper = MatlabWrapper(
            module_name='inheritance',
            top_module_namespace=['gtsam'],
            ignore_classes=[''],
        )
        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        files = [
            'inheritance_wrapper.cpp',
            'MyBase.m',
            'MyTemplateA.m',
            'MyTemplateMatrix.m',
            'MyTemplatePoint2.m',
            'ForwardKinematicsFactor.m',
            'ParentHasTemplateDouble.m',
        ]

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_namespaces(self):
        """
        Test interface file with full namespace definition.
        """
        file = osp.join(self.INTERFACE_DIR, 'namespaces.i')

        wrapper = MatlabWrapper(
            module_name='namespaces',
            top_module_namespace=['gtsam'],
            ignore_classes=[''],
        )

        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        files = [
            'namespaces_wrapper.cpp',
            '+ns1/aGlobalFunction.m',
            '+ns1/ClassA.m',
            '+ns1/ClassB.m',
            '+ns2/+ns3/ClassB.m',
            '+ns2/aGlobalFunction.m',
            '+ns2/ClassA.m',
            '+ns2/ClassC.m',
            '+ns2/overloadedGlobalFunction.m',
            'ClassD.m',
            '+gtsam/Values.m',
        ]

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_special_cases(self):
        """
        Tests for some unique, non-trivial features.
        """
        file = osp.join(self.INTERFACE_DIR, 'special_cases.i')

        wrapper = MatlabWrapper(
            module_name='special_cases',
            top_module_namespace=['gtsam'],
            ignore_classes=[''],
        )
        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        files = [
            'special_cases_wrapper.cpp',
            '+gtsam/GeneralSFMFactorCal3Bundler.m',
            '+gtsam/NonlinearFactorGraph.m',
            '+gtsam/PinholeCameraCal3Bundler.m',
            '+gtsam/SfmTrack.m',
        ]

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_multiple_files(self):
        """
        Test for when multiple interface files are specified.
        """
        file1 = osp.join(self.INTERFACE_DIR, 'part1.i')
        file2 = osp.join(self.INTERFACE_DIR, 'part2.i')

        wrapper = MatlabWrapper(
            module_name='multiple_files',
            top_module_namespace=['gtsam'],
            ignore_classes=[''],
        )

        wrapper.wrap([file1, file2], path=self.MATLAB_ACTUAL_DIR)

        files = [
            'multiple_files_wrapper.cpp',
            '+gtsam/Class1.m',
            '+gtsam/Class2.m',
            '+gtsam/ClassA.m',
        ]

        for file in files:
            actual = osp.join(self.MATLAB_ACTUAL_DIR, file)
            self.compare_and_diff(file, actual)

    def test_scalar_string_and_fixed_width_integer_contracts(self):
        """Check MATLAB scalar generation for strings and fixed-width integers."""
        file = osp.join(self.INTERFACE_DIR, 'matlab_scalar_contracts.i')

        wrapper = MatlabWrapper(
            module_name='matlab_scalar_contracts',
            top_module_namespace=['scalar_contracts'],
            ignore_classes=[''],
        )
        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        with open(osp.join(self.MATLAB_ACTUAL_DIR,
                           'matlab_scalar_contracts_wrapper.cpp'),
                  encoding="UTF-8") as wrapper_file:
            wrapper_cpp = wrapper_file.read()
        with open(osp.join(self.MATLAB_ACTUAL_DIR, '+scalar_contracts',
                           'ScalarContract.m'),
                  encoding="UTF-8") as class_file:
            class_m = class_file.read()
        with open(osp.join(self.MATLAB_ACTUAL_DIR, '+scalar_contracts',
                           'GlobalUint32.m'),
                  encoding="UTF-8") as global_file:
            global_m = global_file.read()

        self.assertNotIn('unwrap_shared_ptr< string >', wrapper_cpp)
        self.assertNotIn('unwrap_shared_ptr< uint32_t >', wrapper_cpp)
        self.assertNotIn('unwrap_shared_ptr< std::uint32_t >', wrapper_cpp)
        self.assertIn('string value = unwrap< string >(in[1]);', wrapper_cpp)
        self.assertIn('uint32_t value = unwrap< uint32_t >(in[1]);',
                      wrapper_cpp)
        self.assertIn('std::uint32_t value = unwrap< std::uint32_t >(in[1]);',
                      wrapper_cpp)
        self.assertIn('out[0] = wrap< uint32_t >', wrapper_cpp)
        self.assertIn("isa(varargin{1},'uint32')", class_m)
        self.assertIn("isa(varargin{1},'uint32')", global_m)
        self.assertNotIn("'uint32_t'", class_m)
        self.assertNotIn("'std.uint32_t'", class_m)

    def test_matlab_ownership_handles_stale_and_upcast_handles(self):
        """Keep generated MATLAB ownership idempotent for stale and virtual handles."""
        file = osp.join(self.INTERFACE_DIR, 'matlab_ownership.i')

        wrapper = MatlabWrapper(
            module_name='matlab_ownership',
            top_module_namespace=[''],
            ignore_classes=[''],
        )
        wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)

        with open(osp.join(self.MATLAB_ACTUAL_DIR,
                           'matlab_ownership_wrapper.cpp'),
                  encoding="UTF-8") as wrapper_file:
            wrapper_cpp = wrapper_file.read()

        self.assertIn(
            "  item = collector_OwnedThing.find(self);\n"
            "  if(item == collector_OwnedThing.end()) {\n"
            "    return;\n"
            "  }\n"
            "  collector_OwnedThing.erase(item);\n"
            "  delete self;\n"
            "  mexUnlock();\n",
            wrapper_cpp)
        self.assertIn(
            "  Shared *self = new Shared(new OwnedThing());\n"
            "  collector_OwnedThing.insert(self);\n"
            "  mexLock();\n"
            "  out[0] = mxCreateNumericMatrix(1, 1, mxUINT32OR64_CLASS, mxREAL);\n",
            wrapper_cpp)
        self.assertIn(
            "void VirtualDerived_upcastFromVoid_", wrapper_cpp)
        self.assertIn(
            "  Shared *self = new Shared(std::static_pointer_cast<VirtualDerived>(*asVoid));\n"
            "  collector_VirtualDerived.insert(self);\n"
            "  mexLock();\n"
            "  *reinterpret_cast<Shared**>(mxGetData(out[0])) = self;\n",
            wrapper_cpp)
        self.assertIn(
            "  Shared *self = new Shared(std::static_pointer_cast<wraptest::NamespacedVirtualDerived>(*asVoid));\n"
            "  collector_wraptestNamespacedVirtualDerived.insert(self);\n"
            "  mexLock();\n"
            "  *reinterpret_cast<Shared**>(mxGetData(out[0])) = self;\n",
            wrapper_cpp)

    def test_matlab_runtime_proxy_construction_failure_cleans_lock(self):
        """Returned shared_ptr proxy construction must release ownership on failure."""
        with open(osp.join(self.TEST_DIR, '..', 'matlab.h'),
                  encoding="UTF-8") as header_file:
            header = header_file.read()

        self.assertIn("constructionError", header)
        self.assertIn("mxArray **constructionError = nullptr", header)
        self.assertIn(
            "mexCallMATLABWithTrap(1, &result, nargin, input, derivedClassName)",
            header)
        self.assertNotIn(
            "mexCallMATLAB(1,&result, nargin, input, derivedClassName);",
            header)
        self.assertIn("if (constructionError)", header)
        self.assertIn("*constructionError = exception;", header)
        self.assertIn('gtwrap::ReportMatlabException(', header)
        self.assertIn('"wrap: failed constructing MATLAB proxy object"', header)
        self.assertIn("if (exception || !result)", header)
        self.assertIn("gtwrap::ReportMatlabException(\n"
                      "                exception, \"wrap: failed constructing MATLAB proxy object\");",
                      header)

    def test_non_const_string_ref_is_rejected(self):
        """Reject mutable string references instead of generating lossy wrappers."""
        file = osp.join(self.INTERFACE_DIR, 'matlab_invalid_string_ref.i')

        wrapper = MatlabWrapper(
            module_name='matlab_invalid_string_ref',
            top_module_namespace=['scalar_contracts'],
            ignore_classes=[''],
        )

        with self.assertRaisesRegex(ValueError,
                                    'Non-const string references'):
            wrapper.wrap([file], path=self.MATLAB_ACTUAL_DIR)


class TestWrapCpp(unittest.TestCase):
    """Test the MATLAB wrapper generating against the modern C++ MEX API.

    Mirrors the C-API codegen tests one-to-one: each module's generated
    ``*_wrapper.cpp`` is compared against a golden fixture under
    ``expected/matlab_cpp``, and every generated ``.m`` file is asserted
    byte-identical to the C-API expected output (the `.m` contract does not
    depend on the MEX API).
    """

    # (module, [interface files], kwargs, [curated output files]).
    # Mirrors the per-module file lists of the C-API tests exactly: the
    # `*_wrapper.cpp` is checked against expected/matlab_cpp, every `.m`
    # against expected/matlab (the `.m` contract is API-independent).
    MODULES = [
        ('geometry', ['geometry.i'],
         dict(top_module_namespace=['gtsam'], use_boost_serialization=True),
         ['geometry_wrapper.cpp', '+gtsam/Point2.m', '+gtsam/Point3.m']),
        ('functions', ['functions.i'], dict(top_module_namespace=['gtsam']),
         ['functions_wrapper.cpp', 'aGlobalFunction.m', 'load2D.m',
          'MultiTemplatedFunctionDoubleSize_tDouble.m',
          'MultiTemplatedFunctionStringSize_tDouble.m',
          'overloadedGlobalFunction.m', 'TemplatedFunctionRot3.m',
          'DefaultFuncInt.m', 'DefaultFuncObj.m', 'DefaultFuncString.m',
          'DefaultFuncVector.m', 'DefaultFuncZero.m', 'setPose.m',
          'EliminateDiscrete.m', 'triangulatePoint3Cal3_S2.m',
          'FindKarcherMeanPoint3.m', 'FindKarcherMeanSO3.m',
          'FindKarcherMeanSO4.m', 'FindKarcherMeanPose3.m']),
        ('class', ['class.i'], dict(top_module_namespace=['gtsam']),
         ['class_wrapper.cpp', 'ForwardKinematics.m', 'FunDouble.m',
          'FunRange.m', 'HessianFactor.m', 'MultipleTemplatesIntDouble.m',
          'MultipleTemplatesIntFloat.m', 'MyFactorPosePoint2.m', 'MyVector3.m',
          'MyVector12.m', 'PrimitiveRefDouble.m',
          'SmartProjectionRigFactorPinholeCameraCal3_S2.m', 'Test.m']),
        ('enum', ['enum.i'], dict(top_module_namespace=['gtsam']),
         ['enum_wrapper.cpp', 'Color.m', 'Pet.m', '+Pet/Kind.m',
          '+gtsam/VerbosityLM.m', '+gtsam/+MCU/Avengers.m',
          '+gtsam/+MCU/GotG.m',
          '+gtsam/+OptimizerGaussNewtonParams/Verbosity.m']),
        ('template', ['templates.i'], dict(top_module_namespace=['gtsam']),
         ['template_wrapper.cpp', 'ScopedTemplateResult.m',
          'TemplatedConstructor.m']),
        ('inheritance', ['inheritance.i'], dict(top_module_namespace=['gtsam']),
         ['inheritance_wrapper.cpp', 'MyBase.m', 'MyTemplateA.m',
          'MyTemplateMatrix.m', 'MyTemplatePoint2.m',
          'ForwardKinematicsFactor.m', 'ParentHasTemplateDouble.m']),
        ('namespaces', ['namespaces.i'], dict(top_module_namespace=['gtsam']),
         ['namespaces_wrapper.cpp', '+ns1/aGlobalFunction.m', '+ns1/ClassA.m',
          '+ns1/ClassB.m', '+ns2/+ns3/ClassB.m', '+ns2/aGlobalFunction.m',
          '+ns2/ClassA.m', '+ns2/ClassC.m', '+ns2/overloadedGlobalFunction.m',
          'ClassD.m', '+gtsam/Values.m']),
        ('special_cases', ['special_cases.i'],
         dict(top_module_namespace=['gtsam']),
         ['special_cases_wrapper.cpp', '+gtsam/GeneralSFMFactorCal3Bundler.m',
          '+gtsam/NonlinearFactorGraph.m', '+gtsam/PinholeCameraCal3Bundler.m',
          '+gtsam/SfmTrack.m']),
        ('multiple_files', ['part1.i', 'part2.i'],
         dict(top_module_namespace=['gtsam']),
         ['multiple_files_wrapper.cpp', '+gtsam/Class1.m', '+gtsam/Class2.m',
          '+gtsam/ClassA.m']),
    ]

    def setUp(self) -> None:
        super().setUp()
        self.TEST_DIR = osp.dirname(osp.realpath(__file__))
        self.INTERFACE_DIR = osp.join(self.TEST_DIR, "fixtures")
        self.MATLAB_C_DIR = osp.join(self.TEST_DIR, "expected", "matlab")
        self.MATLAB_CPP_DIR = osp.join(self.TEST_DIR, "expected", "matlab_cpp")
        self.ACTUAL_DIR = osp.join(self.TEST_DIR, "actual", "matlab_cpp")
        os.makedirs(self.ACTUAL_DIR, exist_ok=True)

    def _generate(self, module_name, srcs, kwargs):
        out = osp.join(self.ACTUAL_DIR, module_name)
        os.makedirs(out, exist_ok=True)
        wrapper = MatlabWrapperCpp(module_name=module_name,
                                   ignore_classes=[''], **kwargs)
        wrapper.wrap([osp.join(self.INTERFACE_DIR, s) for s in srcs], path=out)
        return out

    def _assert_match(self, actual, expected, label):
        if not filecmp.cmp(actual, expected, shallow=False):
            os.system(f"diff {actual} {expected}")
        self.assertTrue(filecmp.cmp(actual, expected, shallow=False),
                        f"Mismatch for {label}")

    def test_cpp_modules_match_expected(self):
        """Each module's .cpp matches the C++ golden and its .m the C target."""
        for module_name, srcs, kwargs, files in self.MODULES:
            with self.subTest(module=module_name):
                out = self._generate(module_name, srcs, kwargs)
                for rel in files:
                    actual = osp.join(out, rel)
                    if rel.endswith('.cpp'):
                        expected = osp.join(self.MATLAB_CPP_DIR, rel)
                    else:
                        expected = osp.join(self.MATLAB_C_DIR, rel)
                    self._assert_match(actual, expected, f"{module_name}/{rel}")

    def test_cpp_ownership_uses_safe_handles(self):
        """C++ target uses typed handles and no manual lock/atexit/mxArray."""
        out = self._generate('matlab_ownership', ['matlab_ownership.i'],
                             dict(top_module_namespace=['']))
        with open(osp.join(out, 'matlab_ownership_wrapper.cpp'),
                  encoding="UTF-8") as f:
            cpp = f.read()

        # Lifetime is owned by the MexFunction instance, not the C lock API.
        for banned in ('mexLock', 'mexUnlock', 'mexAtExit', 'mxCreateNumericMatrix',
                       'mxGetData', 'reinterpret_cast<Shared**>'):
            self.assertNotIn(banned, cpp)

        # Stale-handle protection is preserved.
        self.assertIn(
            "  item = collector_OwnedThing.find(self);\n"
            "  if(item == collector_OwnedThing.end()) {\n"
            "    return;\n"
            "  }\n"
            "  collector_OwnedThing.erase(item);\n"
            "  delete self;\n", cpp)
        # Typed handle helpers + virtual upcast preserved.
        self.assertIn("Shared *self = new Shared(new OwnedThing());", cpp)
        self.assertIn("out[0] = make_handle<Shared>(self);", cpp)
        self.assertIn("Shared *self = get_handle<Shared>(in[0]);", cpp)
        self.assertIn("void VirtualDerived_upcastFromVoid_", cpp)
        self.assertIn(
            "Shared *self = new Shared(std::static_pointer_cast<VirtualDerived>(*asVoid));",
            cpp)

    def test_cpp_scalar_contracts(self):
        """Scalar-vs-pointer contracts are identical to the C target."""
        out = self._generate('matlab_scalar_contracts',
                             ['matlab_scalar_contracts.i'],
                             dict(top_module_namespace=['scalar_contracts']))
        with open(osp.join(out, 'matlab_scalar_contracts_wrapper.cpp'),
                  encoding="UTF-8") as f:
            cpp = f.read()

        self.assertNotIn('unwrap_shared_ptr< string >', cpp)
        self.assertNotIn('unwrap_shared_ptr< uint32_t >', cpp)
        self.assertIn('string value = unwrap< string >(in[1]);', cpp)
        self.assertIn('uint32_t value = unwrap< uint32_t >(in[1]);', cpp)
        self.assertIn('out[0] = wrap< uint32_t >', cpp)

    def test_cpp_matrix_view_arguments(self):
        """C++ MEX target keeps matrix views out of pointer unwrap paths."""
        out = self._generate('matrix_views', ['matrix_views.i'],
                             dict(top_module_namespace=['gtsam']))
        with open(osp.join(out, 'matrix_views_wrapper.cpp'),
                  encoding="UTF-8") as f:
            cpp = f.read()
        with open(osp.join(out, '+gtsam', 'MatrixViewFixture.m'),
                  encoding="UTF-8") as f:
            matlab_content = f.read()
        with open(osp.join(self.TEST_DIR, '..', 'matlab_cpp.h'),
                  encoding="UTF-8") as f:
            header_content = f.read()

        self.assertIn(
            'gtsam::ConstMatrixView points = unwrapMatrixView< gtsam::ConstMatrixView >(in[1]);',
            cpp)
        self.assertIn('obj->acceptView(points);', cpp)
        self.assertNotIn('unwrap_shared_ptr< gtsam::ConstMatrixView >', cpp)
        self.assertNotIn('unwrap< gtsam::ConstMatrixView >', cpp)
        self.assertIn("isa(varargin{1},'double')", matlab_content)
        self.assertIn('unwrapMatrixView', header_content)
        self.assertIn('SPARSE_DOUBLE', header_content)
        self.assertIn('std::numeric_limits<Eigen::Index>', header_content)
        self.assertIn('Stride(m, 1)', header_content)

    def test_cpp_entry_point_structure(self):
        """The generated file exposes a single MexFunction via matlab_cpp.h."""
        out = self._generate('geometry', ['geometry.i'],
                             dict(top_module_namespace=['gtsam'],
                                  use_boost_serialization=True))
        with open(osp.join(out, 'geometry_wrapper.cpp'), encoding="UTF-8") as f:
            cpp = f.read()
        self.assertEqual(cpp.count('class MexFunction'), 1)
        self.assertIn('#include <wrap/matlab_cpp.h>', cpp)
        self.assertIn('~MexFunction() override {\n    try {', cpp)
        self.assertIn('} catch(...) {', cpp)
        self.assertIn('if(!rttiRegistered_) {', cpp)
        self.assertIn('_geometry_RTTIRegister(ctx.engine);', cpp)
        self.assertIn('rttiRegistered_ = true;', cpp)
        self.assertIn('WrapIn in(inputs, 1);', cpp)
        self.assertIn('bool rttiRegistered_ = false;', cpp)
        self.assertNotIn(
            'std::vector<matlab::data::Array> in(inputs.begin() + 1, inputs.end());',
            cpp)
        # mexAdapter.hpp is pulled in once via matlab_cpp.h, not the wrapper.
        self.assertNotIn('mexAdapter.hpp', cpp)


if __name__ == '__main__':
    unittest.main()
