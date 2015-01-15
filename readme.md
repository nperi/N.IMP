n.imp
=====

node-based image processor


Overview appSettings.xml
------------------------

### Basic Structure

```html
<MAIN_SETTINGS>
    <SETTINGS>
        <INPUTS>
            <!-- add inputs here -->
        </INPUTS>
        <VISUAL_LAYERS>
            <!-- add visual layers here --> 
        </VISUAL_LAYERS>
        <MIXERS>
        	<!-- add mixers here --> 
        </MIXERS>
    </SETTINGS>

    <NODE_VIEWS>
    	<!-- create node views from created elements --> 
        <NODE_VIEW name="OUTPUT">
            <NODE name="mainMix" x="20" y="20" guiX="1370" guiY="20" imageScale="1.35" guiWidth="200" />
        </NODE_VIEW>
    </NODE_VIEWS>
</MAIN_SETTINGS>
```

### Inputs

Single Image
```html
<INPUT name="Mask" type="IMAGE" path="images/mask.png"/>
```

Image Sequence
```html
<INPUT name="Mask" type="IMAGE" path="test"
                   isPlaying="true" 
                   bpm="100" 
                   matchBPMtoSequence="true" 
                   multiplier_divider="1"
                   palindrom="false"
                   />
```

Video
```html
<INPUT name="video" type="VIDEO" path="movies/movie.mov"/>
```

Camera
```html
<INPUT name="cam1" type="CAMERA" id="0"/>
```

Particle
```html
<INPUT name="Particle" type="PARTICLE"/>
```

### Visual Layers

IKEDA layer
```html
<VISUAL_LAYER name="V_IKEDA" type="IKEDA" inputSource="Video2"
                    isCanny="true" 
                    isThreshold="true" 
                    isColumns="true" 
                    isInvert="true"
                    pNColumns="4"
                    pCannyX="12"
                    pCannyY="12"
                    pThreshold="12"
                    />
```

Glitch1 layer
```html
<VISUAL_LAYER name="V_GLITCH1" type="GLITCH_1" inputSource="Video2"
                do_CONVERGENCE="false"
                do_GLOW="false"
                do_SHAKER="false"
                de_CUTSLIDER="false"
                do_TWIST="false"
                do_OUTLINE="false"
                do_NOISE="false"
                do_SLITSCAN="false"
                do_SWELL="false"
                do_INVERT="false"
                
                do_CR_HIGHCONTRAST="false"
                do_CR_BLUERAISE="false"
                do_CR_REDRAISE="false"
                do_CR_GREENRAISE="false"
                do_CR_BLUEINVERT="false"
                do_CR_REDINVERT="false"
                do_CR_GREENINVERT="false"/>
```

Glitch2 layer
```html
<VISUAL_LAYER name="V_GLITCH2" type="GLITCH_2" inputSource="Camera1"
                dq="20"
                qn="40"
                dht="80"/>
```

### Mixers

Multi-channel Mixer layer (up to 8 inputs)
```html
<MIXER name="mainMix" type="MULTI_CHANNEL">
                <INPUT_SOURCE name="source1"/>
                <INPUT_SOURCE name="source2"/>
                <INPUT_SOURCE name="source3"/>
                <INPUT_SOURCE name="source4"/>
                <INPUT_SOURCE name="source5"/>
            </MIXER>
```         

Render Pipeline
---------------

###General Render Pipeline

```
 _____  n    ____________   1   ______________
|INPUT|---->|ChannelMixer|---->|ImageProcessor| --
 -----       ------------       --------------    \     ___________    n    _______
                                                    -> |SimpleMixer| ----> |MainMix|
 _____  n    ____________   1   ______________    /     -----------         -------
|INPUT|---->|ChannelMixer|---->|ImageProcessor| --
 -----       ------------       --------------     
```

            