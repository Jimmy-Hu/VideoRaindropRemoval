# VideoRaindropRemoval

Remove raindrop in video.

## Description
In the rain, the monitoring image blurred by the raindrop lines. Because raindrop lines cause some errors to the systems that processing base on camera vision. In the existing visual surveillance systems / monitoring systems, most algorithms first establish a reference background image, and then use a background subtraction method to obtain a foreground image, that is, a moving object detection. However, the accuracy of the background subtraction method can easily be seriously disturbed by changes in the external environment; for example, changes in light intensity, tree shaking, shadow changes and lines caused by raindrops, etc., all of these will affect the accuracy of system analysis and judgment accuracy rate. Therefore, we propose a raindrop removal algorithm that observes the numerical variation of the color model of a raindrop in an image. Moreover, because of the shape of raindrop is like a convex lens, the refraction of the light produced by the raindrop increases the brightness value of the pixel. The quality of image from the static camera in surveillance systems could be improved by this characteristic.

在雨中，監測圖像受雨滴線條干擾而模糊。因為雨滴線條會對基於相機視覺的處理系統造成一些誤差。在現有的視覺監控系統/監視系統中，大多數演算法首先建立參考背景圖像，然後使用背景相減法獲得前景圖像，即移動目標物檢測。然而，背景減除方法的準確性很容易受到外部環境變化的嚴重干擾；例如，光影強度變化，樹木抖動，陰影變化和雨滴線條等引起的變化，所有這些因素皆會影響系統分析的穩定性和判斷準確率。因此，我們提出了一種雨滴去除演算法，該演算法觀察圖像中雨滴的顏色模型的數值變化。且由於雨滴的形狀類似於凸透鏡，雨滴產生的光的折射增加了像素的亮度值，而不會改變色彩的色相(hue)。通過這種特性可以改善監視系統中靜態攝像機的圖像品質。

## License

This program is licensed under GNU General Public License v3.
