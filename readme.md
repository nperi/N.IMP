Architecture proyection software
================================

Video mapping software.


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