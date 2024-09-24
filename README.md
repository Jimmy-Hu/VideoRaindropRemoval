# VideoRaindropRemoval project

Remove raindrop in video.

## Experimental Results

|             Input                 |                  [Deep Detail Network][1]                  |     [MFDNet][2]    |    [MPRNet][3] |  Proposed Algorithm    |
| :---------------------------------------------------: | :---------------------------------------------: | :---------------------------------------------: | :---------------------------------------------: |  :---------------------------------------------: |
| ![InputVideo](https://github.com/Jimmy-Hu/VideoRaindropRemoval/blob/master/resources/Images/gif/Input/1/InputVideo.gif)   |  ![OutputVideo](https://github.com/Jimmy-Hu/VideoRaindropRemoval/blob/master/resources/Images/gif/DeepDetailNetwork/1/Video.gif)    |    ![OutputVideo](https://github.com/Jimmy-Hu/VideoRaindropRemoval/blob/master/resources/Images/gif/MFDNet/1/Video.gif)      |    ![OutputVideo](https://github.com/Jimmy-Hu/VideoRaindropRemoval/blob/master/resources/Images/gif/MPRNet/1/Video.gif)      |    ![OutputVideo](https://github.com/Jimmy-Hu/VideoRaindropRemoval/blob/master/resources/Images/gif/ProposedMethod/BlockSize8x8/1/gaussian_sigma%3D0.1/Video.gif)      |

## Description
在我們的生活中，視訊系統的應用無所不在，例如：視訊監控系統、閉路電視系統等；而能夠獲取清晰的影像是這些系統的共同目標，亦是視訊系統性能評估上的重要指標之一。因模糊的影像常導致資訊判定上的錯誤，特別是近年來自駕車實驗性原型產品的推陳出新，受到廣泛使用之影像物件偵測識別系統可能因下雨時無法獲得清晰的影像，無法正確判別車輛周遭之物件，導致行駛時意外的發生。在解決雨滴影響視訊品質的問題上，透過觀察一段有雨視訊，發現當雨滴經過時，因雨滴類似於凸透鏡可匯聚光線的效果使得像素亮度值暫時有所提升，但不會改變色彩的色相(hue)。因此在現有研究的解決方案中，以偵測及調整「因雨滴經過而產生較高的亮度值」的像素點為目標，進一步改善視訊系統的影像品質。
In the rain, the video monitoring image blurred by the raindrop lines. The raindrop lines causes some errors to the systems that processing base on camera vision. In the existing visual surveillance systems / monitoring systems, most algorithms first establish a reference background image. Use the background subtraction method to obtain a foreground image, which means a moving object detection. However, the external environment can easily interfere with the accuracy of the background subtraction method seriously. For example, changes in light intensity, tree shaking, shadow changes and raindrop lines...etc. All of these variations would affect the accuracy of system analysis and judgment rate. Therefore, we propose a raindrop removal algorithm that can observe the numerical variation of the color model when a raindrop appears in an image. Moreover, because of the shape of the raindrop is like a convex lens. By the theory of refection, when the light goes through the raindrop increases the brightness value of the pixel. In conclusion, the quality of the image from the static camera in surveillance systems could be improved by this characteristic.



## License

This program is licensed under GNU General Public License v3.


[1]: https://ieeexplore.ieee.org/document/8099669
[2]: https://github.com/qwangg/MFDNet
[3]: https://github.com/swz30/MPRNet