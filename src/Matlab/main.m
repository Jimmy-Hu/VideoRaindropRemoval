% DerainOutput8
clear all;
clc;
%close all;
set(groot,'defaultFigureVisible','on');

tic
%%% Parameters

N1 = 8;
N2 = 8;
N3 = 1;

DictionaryFilePath = fullfile('.', 'Dictionary', ['Dictionary', '.mat']);
if ~isfile(DictionaryFilePath)
    fprintf('File %s does not exist!', DictionaryFilePath);
    return;
end
load(DictionaryFilePath);
gaussian_sigma = 0.1;

%%% Main program
i = 1;
RainImagesFilePath = ['.'];
Filename = ['2'];
FullFilename = fullfile(RainImagesFilePath, [Filename '.png']);
if (~isfile(FullFilename))
    fprintf('File %s not found!\n', FullFilename);
    return;
end
OriginInputImage = imread(FullFilename);
InputImage = OriginInputImage(1:floor(size(OriginInputImage, 1) / N1) * N1, 1:floor(size(OriginInputImage, 2) / N2) * N2, :);
ycbcr = rgb2ycbcr(InputImage);
ycbcr(:,:,1) = singleImageDerain(im2double(ycbcr(:,:,1)), Dictionary, gaussian_sigma, N1, N2, N3).*255;
RGBImageOutput = imgaussfilt(ycbcr2rgb(ycbcr), 0.5);
OutputFolder = ['.' filesep 'Output' ];
if ~exist(OutputFolder, 'dir')
    mkdir(OutputFolder);
end
OutputFileName = fullfile(OutputFolder, [Filename '.bmp']);
imwrite(RGBImageOutput, OutputFileName);

%% Write PNG images
OutputFolder = ['.' filesep 'Output' filesep 'PNG' filesep];
if ~exist(OutputFolder, 'dir')
    mkdir(OutputFolder);
end
OutputFileName = sprintf("%s%d%s", OutputFolder, Filename, ".png");
imwrite(RGBImageOutput, OutputFileName);
toc

function [output] = singleImageDerain(input, Dictionary, gaussian_sigma, N1, N2, N3)
    N1Cells = N1 * ones(1,(size(input, 1) / N1));
    N2Cells = N2 * ones(1,(size(input, 2) / N2));
    N3Cells = N3 * ones(1, (size(input, 3) / N3));

    output = mat2cell(input, N1Cells, N2Cells, N3Cells);
    for x = 1:size(output, 1)
        for y = 1:size(output, 2)
            for z = 1:size(output, 3)
                output{x, y, z} = {DCT3D(output{x, y, z})};
            end
        end
        fprintf('%d_%d_%d size DCT: The %d_%d_%d / %d_%d_%d block calculation has done. \n' ,N1, N2, N3, x, y, z, size(output, 1), size(output, 2), size(output, 3));
    end
    
    output = derain(Dictionary, output, gaussian_sigma);

    for x = 1:size(output, 1)
        for y = 1:size(output, 2)
            for z = 1:size(output, 3)
                output{x, y, z} = IDCT3D(cell2mat(output{x, y, z}));
            end
        end
        fprintf('%d_%d_%d size IDCT: The %d_%d_%d / %d_%d_%d block calculation has done. \n' ,N1, N2, N3, x, y, z, size(output, 1), size(output, 2), size(output, 3));
    end
    output = cell2mat(output);
    
    %output = medfilt2(output, [1 3]);
end

function [output] = derain(Dictionary, input, gaussian_sigma)
    output = cell(size(input, 1), size(input, 2), size(input, 3));
    Ylength = size(input, 2);
    Zlength = size(input, 3);
    parfor X = 1:size(input, 1)
        for Y = 1:Ylength
            for Z = 1:Zlength
                SingleDCTCube = cell2mat(input{X, Y, Z});
                SingleDCTCubeOutput = SingleDCTCube + getOffset2(Dictionary, SingleDCTCube, gaussian_sigma);
                output{X, Y, Z} = {SingleDCTCubeOutput};
            end
        end
        fprintf('Derain: The %d_%d_%d / %d_%d_%d block calculation has done. \n' , X, Y, Z, size(input, 1), size(input, 2), size(input, 3));
    end
end

%{
getOffset2 function returns the adjust offset for each cube based on
weighting summation the Y in dictionary rely on the distance between X and
input
%}
function [output] = getOffset2(Dictionary, input, gaussian_sigma)
    gaussian_mean = 0;
    if size(Dictionary.X) ~= size(Dictionary.Y)
        disp("Size of data in dictionary incorrect.");
        output = [];
        return
    end
    [X, Y, Z, DataCount] = size(Dictionary.X);
    distOfEach = zeros(1, DataCount);
    for i = 1:DataCount
        %   Gaussian of distance between X and input
        distOfEach(i) = gaussmf(sum(abs(input - Dictionary.X(:, :, :, i)), 'all'), [gaussian_sigma gaussian_mean]);  
    end
    sumOfDist = sum(distOfEach, 'all');
    output = zeros(X, Y, Z);
    %%% if sumOfDist too small
    if (sumOfDist < 1e-160)
        fprintf("sumOfDist = %d\n", sumOfDist);
        return;
    end
    for i = 1:DataCount
        output = output + Dictionary.Y(:, :, :, i) .* distOfEach(i);
    end
    output = output ./ sumOfDist;
end

%{
getOffset1 function returns the adjust offset for each cube based on
finding the most similar case X to input in dictionary then its Y is the
output
%}
function [output] = getOffset1(Dictionary, input)
    if size(Dictionary.X) ~= size(Dictionary.Y)
        disp("Size of data in dictionary incorrect.");
        output = [];
        return
    end
    [X, Y, Z, DataCount] = size(Dictionary.X);
    distOfEach = zeros(1, DataCount);
    for i = 1:DataCount
        distOfEach(i) = sum(abs(input - Dictionary.X(:, :, :, i)), 'all');
    end
    [Minimum, MinimumDistIndex] = min(distOfEach);
    output = Dictionary.Y(:, :, :, MinimumDistIndex);
    
end
