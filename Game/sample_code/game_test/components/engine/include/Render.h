/*
    Render.h

    Class RenderResource, RenderObject and RenderManager.
*/

#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdint.h>
#include "./Resource.h"

//include library "st7789"
#include "../../st7789/include/st7789.h"
#include "../../fontx/include/fontx.h"
#include "../../pngle/include/pngle.h"
#include "../../decode_png/include/decode_png.h"

#define MAX_TEXT_LENGTH 64
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define TRANSPARENT 0xffff

typedef struct renderManager RenderManager;

/*
    RenderResource class (derived from Resource), indicate a resource which is to be rendered.
*/
typedef struct renderResource
{
    Resource* baseResource;
    uint8_t mRenderResourceID;
    uint16_t mWidth;
    uint16_t mHeight;

    //Image file(for graphic object only)
    uint16_t* mImage;
    //Decoded font file (for text object only)
    //FontxFile mFont[2];

    //Constructor for RenderResource
    void (*new)(struct renderResource*, uint8_t, Resource*, uint16_t, uint16_t);
    
    //sets loadImage function
    void (*setLoadFunction)(struct renderResource*, void (struct renderResource*, const uint16_t*));

    //loadImage function, load image. (for graphic object only)
    void (*loadImage)(struct renderResource*, const uint16_t*);
    //loadFont function, load font. (for text object only)
    //void (*loadFont)(struct renderResource*);

    //sets unloadImage function
    void (*setUnloadFunction)(struct renderResource*, void (struct renderResource*));

    //unload function, set the image pointer to NULL.
    void (*unloadImage)(struct renderResource*);
}RenderResource;

//Constructor for RenderResource
void renderResourceNew(RenderResource*, uint8_t, Resource*, uint16_t, uint16_t);
void renderResourceSetLoadFunction(RenderResource* , void (struct renderResource*, const uint16_t*));
void renderResourceLoadImage(RenderResource*, const uint16_t*);
//void renderResourceLoadFont(RenderResource*);
void renderResourceSetUnloadFunction(RenderResource*, void (struct renderResource*));
void renderResourceUnloadImageDefault(RenderResource*);
void renderResourceUnloadImageHeapFree(RenderResource*);

/*
    RenderObject class, includes a RenderResource and parameters for rendering.
*/
typedef struct renderObject
{
    RenderResource* mRenderResource;

    int16_t mPrePosX;
    int16_t mPrePosY;
    int16_t mPosX;
    int16_t mPosY;
    uint16_t mColor;
    uint8_t mPreVisible;
    uint8_t mVisible;
    //float mScale;

    //Display text (for text object only)
    //uint8_t mText[MAX_TEXT_LENGTH];

    //next and previous RenderObject
    struct renderObject* nextObj;
    struct renderObject* prevObj;

    //constructor of RenderObject
    void (*new)(struct renderObject*, RenderResource*, int16_t, int16_t, uint8_t);
    
    //Gets position (x direction)
    int16_t (*getPosX)(struct renderObject*);

    //Gets position (y direction)
    int16_t (*getPosY)(struct renderObject*);

    //Gets visible
    uint8_t (*getVisible)(struct renderObject*);
    
    //Sets image
    void (*setImage)(struct renderObject*, RenderResource*);
    
    //Sets position
    void (*setPos)(struct renderObject*, int16_t, int16_t);
    
    //Sets color (default: WHITE)
    void (*setColor)(struct renderObject*, uint16_t);

    //Sets visible
    void (*setVisible)(struct renderObject*, uint8_t);

    //Sets text (for text object only)
    //void (*setText)(struct renderObject*, char[]);

    //Adds RenderObject to the render list, which is to be rendered.
    void (*add)(struct renderObject*, RenderManager*);
    //Removes RenderObject in the list.
    void (*remove)(struct renderObject*, RenderManager*);

    //Renders object.
    void (*render)(struct renderObject*, RenderManager*);
}RenderObject;

//constructor of RenderObject
void renderObjectNew(RenderObject*, RenderResource*, int16_t, int16_t, uint8_t);
int16_t renderObjectGetPosX(RenderObject*);
int16_t renderObjectGetPosY(RenderObject*);
uint8_t renderObjectGetVisible(RenderObject*);
void renderObjectSetImage(RenderObject*, RenderResource*);
void renderObjectSetPos(RenderObject*, int16_t, int16_t);
void renderObjectSetColor(RenderObject*, uint16_t);
void renderObjectSetVisible(RenderObject*, uint8_t);
//void renderObjectSetText(RenderObject*, char[]);
void renderObjectAdd(RenderObject*, RenderManager*);
void renderObjectRemove(RenderObject*, RenderManager*);
void renderObjectRender(RenderObject*, RenderManager*);

/*
    RenderManager class, manages RenderResource and RenderObject objects.
*/
typedef struct renderManager
{
    TFT_t TFT_t;
    uint16_t *mScreen;
    uint8_t mRenderResourceCount;
    uint8_t mRenderObjectCount;
    void (*new)(struct renderManager*);

    RenderResource mRenderResources[MAX_OBJECT_COUNT];
    RenderObject mInitRenderObject; //a sentinel of the linked list

    //Finds RenderResource by its ID, returns NULL if not found.
    RenderResource* (*findRenderResourceByID)(struct renderManager*, uint8_t);
    //Finds RenderResource by its name, returns NULL if not found.
    RenderResource* (*findRenderResourceByName)(struct renderManager*, char[]); 

    //Adds Image type RenderResource, whose base class is also registered in the ResourceManager.
    void (*addImage)(struct renderManager*, ResourceManager*, char[], const uint16_t*, uint16_t, uint16_t);
    void (*addImageByFunction)(struct renderManager*, ResourceManager*, char[], void (RenderResource*, uint16_t*), uint16_t, uint16_t);
    //Adds Text type RenderResource (aka. font), whose base class is also registered in the ResourceManager.
    //void (*addFont)(struct renderManager*, ResourceManager*, char[]);

    //Adds RenderObject to the render list, which is to be rendered.
    void (*addObject)(struct renderManager*, RenderObject*);
    //Removes RenderObject in the list.
    void (*removeObject)(struct renderManager*, RenderObject*);

    void (*copy)(struct renderManager*, RenderObject*, int16_t, int16_t, uint16_t, uint16_t);
    void (*readDown)(struct renderManager*, RenderObject*);
    void (*readUp)(struct renderManager*, RenderObject*);
    void (*readFull)(struct renderManager*, RenderObject*);
    
    //Clear the current render list.
    void (*clear)(struct renderManager*);
    //Evaluate the data to be rendered, and send to st7789 buffer.
    void (*reRender)(struct renderManager*, RenderObject*);

    //Update the screen.
    void (*update)(struct renderManager*);
    //Render all texts in the linked list (IN ORDER).
    //void (*renderAllText)(struct renderManager*);
    //Render all objects in the linked list (IN ORDER).
    //void (*renderAllObject)(struct renderManager*);
}RenderManager;

RenderManager* newRenderManager();
void deleteRenderManager(RenderManager*);
//constructor of RenderManager
void renderManagerNew(RenderManager*);
RenderResource* renderManagerFindRenderResourceByID(RenderManager*, uint8_t); 
RenderResource* renderManagerFindRenderResourceByName(RenderManager*, char[]); 
void renderManagerAddImage(RenderManager*, ResourceManager*, char[], const uint16_t*, uint16_t, uint16_t);
void renderManagerAddImageByFunction(RenderManager*, ResourceManager*, char[], void (RenderResource*, uint16_t*), uint16_t, uint16_t);
//void renderManagerAddFont(RenderManager*, ResourceManager*, char[]);
void renderManagerAddObject(RenderManager*, RenderObject*);   
void renderManagerRemoveObject(RenderManager*, RenderObject*); 

void renderManagerCopy(RenderManager*, RenderObject*, int16_t, int16_t, uint16_t, uint16_t);
void renderManagerReadDown(RenderManager*, RenderObject*);
void renderManagerReadUp(RenderManager*, RenderObject*);
void renderManagerReadFull(RenderManager*, RenderObject*);

void renderManagerClear(RenderManager*);
void renderManagerReRender(RenderManager*, RenderObject*);
void renderManagerUpdate(RenderManager*);
//void renderManagerRenderAllText(RenderManager*);
//void renderManagerRenderAllObject(RenderManager*);

#endif /* _RENDER_H_ */