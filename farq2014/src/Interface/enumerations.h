//
//  enumerations.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/28/15.
//
//

#ifndef NIMP_Prototipe_enumerations_h
#define NIMP_Prototipe_enumerations_h

#define BUFFER_SIZE 256
#define NUM_WINDOWS 80

#define NODE_WIDTH 640  //1024
#define NODE_HEIGHT 480 //576

#define MENU_ITEM_SIZE 20
#define MENU_ITEM_PADDING 7
#define MENU_HEIGHT 42
#define MENU_TOP_PADDING -17
#define RIGHT_MENU_WIDTH 25
#define RIGHT_MENU_LONG_WIDTH 250

#define COMPOSER_EVENT_PRIORITY 202
#define PATCH_EVENT_PRIORITY 203
#define SCROLL_BAR_EVENT_PRIORITY 201

#define SCALE_SENSITIVITY 0.001f
#define ZOOM_SENSITIVITY .001f
#define ZOOM_DIFF .5f
#define ZOOM_UNIT 1.f

#define KEY_SCROLL_SENSITIVITY 10.f
#define SCROLL_TOLERANCE 10
#define GRIP_TOLERANCE .002f
#define SCROLL_BAR_WIDTH 13.f


enum nodeLinkType
{
    STRAIGHT_LINKS = 0,
    CURVE_LINKS = 1,
    PATH_LINKS = 2
};

#endif
