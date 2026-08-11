function tests = testImportWrapBuildDir
%% SIGNATURE
% tests = testImportWrapBuildDir
% -------------------------------------------------------------------------------------------------------------
%% DESCRIPTION
% Verify MATLAB wrapper build-directory discovery and returned path reporting.
% -------------------------------------------------------------------------------------------------------------
%% INPUT
% -------------------------------------------------------------------------------------------------------------
%% OUTPUT
% tests                         MATLAB function-test array.
% -------------------------------------------------------------------------------------------------------------
%% CHANGELOG
% 29-07-2026  Pietro Califano, Codex     First implementation.
% -------------------------------------------------------------------------------------------------------------
%% DEPENDENCIES
% ImportWrapBuildDir
% -------------------------------------------------------------------------------------------------------------

tests = functiontests(localfunctions);
end

function testNamespaceTakesPrecedenceOverRepositoryFolder(testCase)
%% SIGNATURE
% testNamespaceTakesPrecedenceOverRepositoryFolder(testCase)
% -------------------------------------------------------------------------------------------------------------
%% DESCRIPTION
% Confirm that an explicitly supplied namespace takes precedence when both the
% namespace and repository-name wrapper folders exist.
% -------------------------------------------------------------------------------------------------------------
%% INPUT
% testCase                      MATLAB unit-test case.
% -------------------------------------------------------------------------------------------------------------
%% OUTPUT
% -------------------------------------------------------------------------------------------------------------
%% CHANGELOG
% 29-07-2026  Pietro Califano, Codex     First implementation.
% -------------------------------------------------------------------------------------------------------------
%% DEPENDENCIES
% ImportWrapBuildDir
% -------------------------------------------------------------------------------------------------------------

charTemporaryRoot = tempname;
charBuildFolder = fullfile(charTemporaryRoot, 'spectral_raytracer', 'build');
charNamespaceWrapper = fullfile(charBuildFolder, 'wrap', 'spectra_rt');
charRepositoryWrapper = fullfile(charBuildFolder, 'wrap', 'spectral_raytracer');
charMexFolder = fullfile(charBuildFolder, 'wrap', 'spectra_rt_mex');

mkdir(fullfile(charNamespaceWrapper, '+spectra_rt'));
mkdir(fullfile(charRepositoryWrapper, '+spectral_raytracer'));
mkdir(charMexFolder);
objCleanup = onCleanup(@() CleanupTemporaryBuild_(charTemporaryRoot, ...
                                                  charNamespaceWrapper, ...
                                                  charMexFolder));

charAddedPaths = ImportWrapBuildDir({charBuildFolder}, {"spectra_rt"});
cellAddedPaths = strsplit(char(charAddedPaths), ';');

verifyEqual(testCase, cellAddedPaths, {charNamespaceWrapper, charMexFolder});
end

function testNamespaceResolvesRenamedWrapperFolder(testCase)
%% SIGNATURE
% testNamespaceResolvesRenamedWrapperFolder(testCase)
% -------------------------------------------------------------------------------------------------------------
%% DESCRIPTION
% Confirm that an import namespace resolves a wrapper folder whose name differs
% from the repository containing the build directory.
% -------------------------------------------------------------------------------------------------------------
%% INPUT
% testCase                      MATLAB unit-test case.
% -------------------------------------------------------------------------------------------------------------
%% OUTPUT
% -------------------------------------------------------------------------------------------------------------
%% CHANGELOG
% 29-07-2026  Pietro Califano, Codex     First implementation.
% -------------------------------------------------------------------------------------------------------------
%% DEPENDENCIES
% ImportWrapBuildDir
% -------------------------------------------------------------------------------------------------------------

charTemporaryRoot = tempname;
charBuildFolder = fullfile(charTemporaryRoot, 'spectral_raytracer', 'build');
charWrapperFolder = fullfile(charBuildFolder, 'wrap', 'spectra_rt');
charMexFolder = fullfile(charBuildFolder, 'wrap', 'spectra_rt_mex');

mkdir(fullfile(charWrapperFolder, '+spectra_rt'));
mkdir(charMexFolder);
objCleanup = onCleanup(@() CleanupTemporaryBuild_(charTemporaryRoot, ...
                                                  charWrapperFolder, ...
                                                  charMexFolder));

charAddedPaths = ImportWrapBuildDir({charBuildFolder}, {"spectra_rt"});
cellAddedPaths = strsplit(char(charAddedPaths), ';');

verifyEqual(testCase, cellAddedPaths, {charWrapperFolder, charMexFolder});
end

function testRepositoryFolderUsedWhenNamespaceFolderMissing(testCase)
%% SIGNATURE
% testRepositoryFolderUsedWhenNamespaceFolderMissing(testCase)
% -------------------------------------------------------------------------------------------------------------
%% DESCRIPTION
% Confirm that the repository-name wrapper folder remains the compatibility
% fallback when the supplied namespace has no matching wrapper folder.
% -------------------------------------------------------------------------------------------------------------
%% INPUT
% testCase                      MATLAB unit-test case.
% -------------------------------------------------------------------------------------------------------------
%% OUTPUT
% -------------------------------------------------------------------------------------------------------------
%% CHANGELOG
% 29-07-2026  Pietro Califano, Codex     First implementation.
% -------------------------------------------------------------------------------------------------------------
%% DEPENDENCIES
% ImportWrapBuildDir
% -------------------------------------------------------------------------------------------------------------

charTemporaryRoot = tempname;
charBuildFolder = fullfile(charTemporaryRoot, 'spectral_raytracer', 'build');
charRepositoryWrapper = fullfile(charBuildFolder, 'wrap', 'spectral_raytracer');
charMexFolder = fullfile(charBuildFolder, 'wrap', 'spectra_rt_mex');

mkdir(fullfile(charRepositoryWrapper, '+spectral_raytracer'));
mkdir(charMexFolder);
objCleanup = onCleanup(@() CleanupTemporaryBuild_(charTemporaryRoot, ...
                                                  charRepositoryWrapper, ...
                                                  charMexFolder));

charAddedPaths = ImportWrapBuildDir({charBuildFolder}, {"spectra_rt"});
cellAddedPaths = strsplit(char(charAddedPaths), ';');

verifyEqual(testCase, cellAddedPaths, {charRepositoryWrapper, charMexFolder});
end

function CleanupTemporaryBuild_(charTemporaryRoot, charWrapperFolder, charMexFolder)
%% SIGNATURE
% CleanupTemporaryBuild_(charTemporaryRoot, charWrapperFolder, charMexFolder)
% -------------------------------------------------------------------------------------------------------------
%% DESCRIPTION
% Remove temporary wrapper paths and their generated directory tree.
% -------------------------------------------------------------------------------------------------------------
%% INPUT
% charTemporaryRoot             Temporary test root.
% charWrapperFolder             Temporary MATLAB wrapper folder.
% charMexFolder                 Temporary MEX wrapper folder.
% -------------------------------------------------------------------------------------------------------------
%% OUTPUT
% -------------------------------------------------------------------------------------------------------------
%% CHANGELOG
% 29-07-2026  Pietro Califano, Codex     First implementation.
% -------------------------------------------------------------------------------------------------------------
%% DEPENDENCIES
% -------------------------------------------------------------------------------------------------------------

cellCurrentPaths = strsplit(path, pathsep);
if any(strcmp(cellCurrentPaths, charWrapperFolder))
    rmpath(charWrapperFolder);
end
if any(strcmp(cellCurrentPaths, charMexFolder))
    rmpath(charMexFolder);
end
if isfolder(charTemporaryRoot)
    rmdir(charTemporaryRoot, 's');
end
end
