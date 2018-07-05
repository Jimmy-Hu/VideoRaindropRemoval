# VideoRaindropRemoval

Remove raindrop in video.

## Description
In the rain, the monitoring image blurred by the raindrop lines. Because raindrop lines cause some errors to the systems that processing base on camera vision. In the existing visual surveillance systems / monitoring systems, most algorithms first establish a reference background image, and then use a background subtraction method to obtain a foreground image, that is, a moving object detection. However, the accuracy of the background subtraction method can easily be seriously disturbed by changes in the external environment; for example, changes in light intensity, tree shaking, shadow changes and lines caused by raindrops, etc., all of these will affect the accuracy of system analysis and judgment accuracy rate. Therefore, we propose a raindrop removal algorithm that observes the numerical variation of the color model of a raindrop in an image. Moreover, because of the shape of raindrop is like a convex lens, the refraction of the light produced by the raindrop increases the brightness value of the pixel. The quality of image from the static camera in surveillance systems could be improved by this characteristic.

## License

This program is licensed under GNU General Public License v3.
