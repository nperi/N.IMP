N.IMP
=====

Node-based Image Processor

Building
---------------
To build this project go to /bin/data path and run setup.sh script


Description
---------------
N.IMP is an audiovisual real-time content generator made in Open Frameworks. It is a node-based arquitecture which has different types of inputs to feed content to the application, then layers and mixers to process them and generates a new output. It has a syphon node to export the generated content.

For the moment it's only available for mac.


### Inputs

- Image and video Sequence

Drag and drop an image or video file to the workspace to create an input of this type. It supports HAP codec for videos.

- Camera

Recognizes every webcam connected to the host.

- Particle Generator

Random particle generator. Control lifetime and gravity/repulsion points.

- Syphon

Consume content published in syphon.

- Audio/OSC/MIDI

Control other node's parameters with audio or midi/osc messages.


### Visual Layers

- IKEDA Layer, Glitch1 Layer, Glitch2 Layer, Image Processor

Greyscale effects, Canny's algorithm, several other effects.

- Shaders

Create your own shaders to apply different transformations. It supports GLSL version 120


### Mixers

- Multi-Channel Switch (up to 8 inputs), Mix Simple Blend, Mix Mask

Mix different inputs and apply transformations


### Outputs

- Syphon Output

Export generated content to syphon server.



### Screenshot
![alt tag](/screenShot.png?raw=true "Demo")


### Used OF addons
Forked this addons and made our customization.

https://github.com/patriciogonzalezvivo/ofxComposer

https://github.com/rezaali/ofxUI

https://github.com/neilmendoza/ofxPostProcessing

https://github.com/astellato/ofxSyphon

https://github.com/danomatika/ofxMidi

https://github.com/jens-a-e/ofxMultiTouchPad

https://github.com/Akira-Hayasaka/ofxPSBlend

https://github.com/maxillacult/ofxPostGlitch

https://github.com/bangnoise/ofxHapPlayer

https://github.com/brinoausrino/ofxImageSequencePlayer

https://github.com/2bbb/ofxJpegGlitch

https://github.com/kronick/ofxAVFVideoPlayer

https://github.com/leozimmerman/ofxFilterbank

