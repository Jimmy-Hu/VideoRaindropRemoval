% PSNR calculation
clear all;
clc;
%close all;
set(groot,'defaultFigureVisible','on');
com.mathworks.mlservices.MLCommandHistoryServices.removeAll;

VideoLength = 100;

for DataIndex = 1:2
    PSNR_result = zeros(1, VideoLength);
    for LoopNumberForLoadingAllFrames = 1:VideoLength
        InputFileName = sprintf("%s%d%s%d%s", "./", DataIndex, "/", LoopNumberForLoadingAllFrames, ".bmp");
        if (isfile(InputFileName) == false)
            continue;
        end
        image = imread(InputFileName);
        
        GroundTruthFileName = sprintf("%s%d%s", "./../InputImages/GroundTruthImages/", DataIndex, ".png");
        if (isfile(GroundTruthFileName) == false)
            continue;
        end
        imageGroundTruth = imread(GroundTruthFileName);
        
        PSNR_result(LoopNumberForLoadingAllFrames) = psnr(image, imageGroundTruth);
    end
    filename = "PSNR_" + DataIndex + ".xlsx";
    writematrix(PSNR_result', filename, 'Sheet', 1, 'Range', 'A1');
end