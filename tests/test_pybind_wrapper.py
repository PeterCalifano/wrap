"""
Unit test for Pybind wrap program
Author: Matthew Sklar, Varun Agrawal
Date: February 2019
"""

# pylint: disable=import-error, wrong-import-position, too-many-branches

import filecmp
import os
import os.path as osp
import sys
import unittest

sys.path.append(osp.dirname(osp.dirname(osp.abspath(__file__))))
sys.path.append(
    osp.normpath(osp.abspath(osp.join(__file__, '../../../build/wrap'))))

import gtwrap.interface_parser as parser
import gtwrap.template_instantiator as instantiator
from gtwrap.pybind_wrapper import PybindWrapper

sys.path.append(osp.dirname(osp.dirname(osp.abspath(__file__))))


class TestWrap(unittest.TestCase):
    """Tests for Python wrapper based on Pybind11."""
    TEST_DIR = osp.dirname(osp.realpath(__file__))
    INTERFACE_DIR = osp.join(TEST_DIR, 'fixtures')

    def wrap_content(self, content, module_name, output_dir):
        """
        Common function to wrap content.
        """
        module = parser.Module.parseString(content)

        instantiator.instantiate_namespace_inplace(module)

        with open(osp.join(self.TEST_DIR,
                           "pybind_wrapper.tpl")) as template_file:
            module_template = template_file.read()

        # Create Pybind wrapper instance
        wrapper = PybindWrapper(module=module,
                                module_name=module_name,
                                use_boost=False,
                                top_module_namespaces=[''],
                                ignore_classes=[''],
                                module_template=module_template)

        cc_content = wrapper.wrap()

        output = osp.join(self.TEST_DIR, output_dir, module_name + ".cpp")

        if not osp.exists(osp.join(self.TEST_DIR, output_dir)):
            os.mkdir(osp.join(self.TEST_DIR, output_dir))

        with open(output, 'w') as f:
            f.write(cc_content)

        return output

    def compare_and_diff(self, file, actual):
        """
        Compute the comparison between the expected and actual file,
        and assert if diff is zero.
        """
        expected = osp.join(self.TEST_DIR, 'expected', 'python', file)
        success = filecmp.cmp(actual, expected)

        if not success:
            os.system("diff {} {}".format(actual, expected))
        self.assertTrue(success, "Mismatch for file {0}".format(file))

    def test_geometry(self):
        """
        Check generation of python geometry wrapper.
        python3 ../pybind_wrapper.py --src geometry.h --module_name
            geometry_py --out output/geometry_py.cc
        """
        with open(osp.join(self.INTERFACE_DIR, 'geometry.i'), 'r') as f:
            content = f.read()

        output = self.wrap_content(content, 'geometry_py',
                                   osp.join('actual', 'python'))

        self.compare_and_diff('geometry_pybind.cpp', output)

    def test_functions(self):
        """Test interface file with function info."""
        with open(osp.join(self.INTERFACE_DIR, 'functions.i'), 'r') as f:
            content = f.read()

        output = self.wrap_content(content, 'functions_py',
                                   osp.join('actual', 'python'))

        self.compare_and_diff('functions_pybind.cpp', output)

    def test_class(self):
        """Test interface file with only class info."""
        with open(osp.join(self.INTERFACE_DIR, 'class.i'), 'r') as f:
            content = f.read()

        output = self.wrap_content(content, 'class_py',
                                   osp.join('actual', 'python'))

        self.compare_and_diff('class_pybind.cpp', output)

    def test_inheritance(self):
        """Test interface file with class inheritance definitions."""
        with open(osp.join(self.INTERFACE_DIR, 'inheritance.i'), 'r') as f:
            content = f.read()

        output = self.wrap_content(content, 'inheritance_py',
                                   osp.join('actual', 'python'))

        self.compare_and_diff('inheritance_pybind.cpp', output)

    def test_namespaces(self):
        """
        Check generation of python wrapper for namespace definition.
        python3 ../pybind_wrapper.py --src namespaces.i --module_name
            namespaces_py --out output/namespaces_py.cpp
        """
        with open(osp.join(self.INTERFACE_DIR, 'namespaces.i'), 'r') as f:
            content = f.read()

        output = self.wrap_content(content, 'namespaces_py',
                                   osp.join('actual', 'python'))

        self.compare_and_diff('namespaces_pybind.cpp', output)


if __name__ == '__main__':
    unittest.main()
