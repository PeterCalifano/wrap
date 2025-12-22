function [charAddedPaths] = ImportWrapBuildDir(cellBuildFolders, cellImportNamespaces, kwargs)
arguments
    cellBuildFolders             (1,:) {mustBeA(cellBuildFolders, ["cell", "string"])}     = {"../gtsam/build", "../lib/gtsam_spaceNav/build"}
    cellImportNamespaces         (1,:) {mustBeA(cellImportNamespaces, ["cell", "string"])} = {"gtsam"}
end
arguments
    kwargs.bImportSubPackages    (1,1) logical = false
end
%% SIGNATURE
% [charAddPaths] = ImportWrapBuildDir(cellBuildFolders, cellImportNamespaces)
% -------------------------------------------------------------------------------------------------------------
%% DESCRIPTION
% Function automating the addpath and import operations for C and C++ libraries wrapped using GTwrap
% library. Default values of inputs assume use in nav-backend repository. It supports recursive import of
% package folders within other packages.
% -------------------------------------------------------------------------------------------------------------
%% INPUT
% cellBuildFolders             (1,:) {mustBeA(cellBuildFolders, ["cell", "string"])}     = {"../gtsam/build", "../lib/gtsam_spaceNav/build"}
% cellImportNamespaces         (1,:) {mustBeA(cellImportNamespaces, ["cell", "string"])} = {"gtsam"}
% -------------------------------------------------------------------------------------------------------------
%% OUTPUT
% charAddedPaths
% -------------------------------------------------------------------------------------------------------------
%% CHANGELOG
% 01-02-2025    Pietro Califano     Function implementation, assuming imported builds uses GTwrap library
% 07-08-2025    Pietro Califano     Improve automatic build dir import function
% 14-12-2025    Pietro Califano     Extend build/wrap folder detection implementation
% -------------------------------------------------------------------------------------------------------------
%% DEPENDENCIES
% [-]
% -------------------------------------------------------------------------------------------------------------

cellAddedPaths = cell(1, 2*length(cellBuildFolders));  % To collect the paths added
idAddCount = 0;

if nargin == 0
    % Default usage for nav-backend
    charCurrentDir = pwd;
    % Get directory of this function and cd to it
    cd(fileparts(mfilename('fullpath')));
end

% Convert string arrays to cell arrays for easy iteration
if isstring(cellBuildFolders)
    cellBuildFolders = cellstr(cellBuildFolders);
end
if isstring(cellImportNamespaces)
    cellImportNamespaces = cellstr(cellImportNamespaces);
end

ui32NumOfPathsToAdd = length(cellBuildFolders);
ui32NumOfLibsToImport = length(cellImportNamespaces);

% Get absolute paths from relative and check it exists
for idPath = 1:ui32NumOfPathsToAdd
    charBuildFolder = cellBuildFolders{idPath};
    try
        mustBeFolder(charBuildFolder); % Ensure folder exists in the first place
    catch
        error('Folder %s is not found or does not exist.', charBuildFolder)
    end

    charPrevDir = cd(charBuildFolder);
    cellBuildFolders{idPath} = pwd;
    cd(charPrevDir);
end

% Loop over each provided build folder
cellWrap        = cell(ui32NumOfPathsToAdd, 1);
cellLibraryName = cell(ui32NumOfPathsToAdd, 1);

for idPath = 1:ui32NumOfPathsToAdd

    % Get folder path
    charBuildFolder = cellBuildFolders{idPath};

    if isfolder(charBuildFolder)

        % Get name of the library (directory name)
        % Split the path into parts
        charPathParts = strsplit(charBuildFolder, filesep);

        % Remove empty parts (which can occur with trailing file separators)
        charPathParts = charPathParts(~cellfun('isempty', charPathParts));

        % Find the folder name that contains the name of the library and not "mex"
        cellLibraryName{idPath} = charPathParts{end-1};

        if ~isfolder(fullfile(charBuildFolder, "wrap", cellLibraryName{idPath}))
            % Library may have different name. Check if a folder matches the name in some part

            strDirStruct = dir(fullfile(charBuildFolder, "wrap"));
            charIsValidDir = [strDirStruct.isdir] & ~ismember({strDirStruct.name},{'.','..'});
            strDirStruct = strDirStruct(charIsValidDir);

            bFound = false;

            % "contains" heuristic (excluding *_mex*)
            for ui32DirID = 1:numel(strDirStruct)
                charDirName = strDirStruct(ui32DirID).name;

                if ( contains(charDirName, cellLibraryName{idPath}) || ...
                        contains(cellLibraryName{idPath}, charDirName) ) && ...
                        ~contains(charDirName, "mex")

                    cellLibraryName{idPath} = charDirName;
                    bFound = true;
                    break
                end
            end

            % Fallback: if NOT found, detect unique folder (excluding *_mex*)
            %    that contains exactly one .cpp: "<folder>_wrapper.cpp"
            if not(bFound)
                cellCandidates = {};

                for ui32DirID = 1:numel(strDirStruct)
                    charDirName = strDirStruct(ui32DirID).name;
                    if contains(charDirName, "mex")
                        continue
                    end

                    charCandidateFolder = fullfile(charBuildFolder, "wrap", charDirName);

                    strCpp = dir(fullfile(charCandidateFolder, "*.cpp"));
                    if numel(strCpp) ~= 1
                        continue
                    end

                    charExpected = charDirName + "_wrapper.cpp";
                    if strcmp(string(strCpp(1).name), charExpected)
                        cellCandidates{end+1} = charDirName; %#ok<AGROW>
                    end
                end

                if isscalar(cellCandidates)
                    cellLibraryName{idPath} = cellCandidates{1};
                    bFound = true;
                end
            end
        end
        
        % Define subdirectories to add
        cellWrap{idPath}    = fullfile(charBuildFolder, "wrap", cellLibraryName{idPath});

        % Search for a target path ending in mex
        charMexFolder = dir(fullfile(charBuildFolder, "wrap", '*_mex'));
        assert(not(isempty(charMexFolder)), sprintf('ERROR: no folder in %s matching pattern: "*_mex". Have you built library with MATLAB wrapper support?', fullfile(charBuildFolder, "wrap")) );
        assert(isscalar(charMexFolder), sprintf('ERROR: found multiple folders in %s matching pattern: "*_mex"', fullfile(charBuildFolder, "wrap")) );
        charWrapMex   = fullfile(charBuildFolder, "wrap", charMexFolder(1).name );
        
        % Add charWrap if it exists
        if isfolder(cellWrap{idPath})

            addpath(cellWrap{idPath});
            idAddCount = idAddCount + 1;
            cellAddedPaths{idAddCount} = cellWrap{idPath};
        else
            warning("Folder '%s' not found.", cellWrap{idPath});
        end
        
        % Add charWrapMex if it exists
        if isfolder(charWrapMex)
            
            addpath(charWrapMex);
            idAddCount = idAddCount + 1;
            cellAddedPaths{idAddCount} = charWrapMex;

        else
            warning("Folder '%s' not found.", charWrapMex);
        end
    else
        error("Build folder '%s' not found.", charBuildFolder);
    end
end

% Define char list of added paths wiht ; delimiter 
charAddedPaths = strjoin(string(cellAddedPaths),';');

% Iterare over import namespaces to import them (make sure there are no conflicting definitions!)

% Import the specified namespaces
for idImport = 1:ui32NumOfLibsToImport

    charImportName = cellImportNamespaces{idImport};

    try
        % Using eval to dynamically call import
        evalin('caller', sprintf('import %s.*', charImportName));

        % Discover and import subpackages if requested
        if kwargs.bImportSubPackages
            % FIXME implementation includes the root folder as well but it should not. 

            for idLibrary = 1:length(cellWrap)
                % FIXME cellSubPackageNames must contain only the name of the subpackage
                [cellSubPackageNames] = DiscoverSubPackages(cellWrap{idLibrary}, cellLibraryName{idLibrary});
                % Import each discovered subpackage in the caller workspace
                for idSubPack = 1:length(cellSubPackageNames)

                    % Check if folder exists before attempting import
                    % DEVNOTE import limited to 1 layer of recursion only
                    if isfolder( fullfile(cellWrap{idLibrary}, ...
                                          strcat("+", cellLibraryName{idLibrary}), ...
                                          strcat("+", cellSubPackageNames{idSubPack}) ) )

                        evalin('caller', sprintf('import %s.%s.*', charImportName, cellSubPackageNames{idSubPack}) );
                    end
                end
            end
        end

    catch ME
        warning("Failed to import namespace '%s': %s", charImportName, ME.message);
    end
    
end

if nargin == 0
    % Default usage for nav-backend
    cd(charCurrentDir)
end

%% LOCAL FUNCTION
    function [cellSubPackageNames] = DiscoverSubPackages(charRootPackage, charLibraryName)
        % Convert package name to folder path
        % charRootFolder = strrep(charRootPackage, '.', filesep);
        charRootFolder = charRootPackage;
        if ~isfolder(charRootFolder)
            error('Package folder %s not found.', charRootFolder);
        end

        % Recursively find all subpackages
        cellSubPackageNames = FindSubPackages(charRootFolder);

        % Remove library name from subpackages
        cellSubPackageNames = cellSubPackageNames(~strcmpi(cellSubPackageNames, charLibraryName));
    end

% Recursive discovery function
    function [cellSubPackages] = FindSubPackages(charBaseFolder)

        cellSubPackages = {}; % Start with the base package
        strDirContents = dir(charBaseFolder);

        for idIdx = 1:length(strDirContents)

            if strDirContents(idIdx).isdir && startsWith(strDirContents(idIdx).name, '+')

                charSubPackage = [strDirContents(idIdx).name(2:end)]; % Remove "+"
                charSubFolder = fullfile(charBaseFolder, strDirContents(idIdx).name);

                % Recursively find subpackages
                cellSubPackages = [cellSubPackages, charSubPackage, FindSubPackages(charSubFolder)]; %#ok<AGROW>
            end
        end
    end

end
