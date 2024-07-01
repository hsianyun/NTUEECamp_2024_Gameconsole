/*
    RenderManager.h

    Class RenderResource, RenderObject and RenderManager.
*/

#ifndef _RENDERMANAGER_H_
#define _RENDERMANAGER_H_

#include <stdint.h>
#include "./ResourceManager.h"
#define MAX_OBJECT_COUNT 200

/*
    RenderResource class (derived from Resource), indicate a resource which is to be rendered.
*/
typedef struct renderResource
{
    Resource* baseResource;
    uint16_t mWidth;
    uint16_t mHeight;
    uint8_t* mImage;

    //Constructor for RenderResource
    void (*new)(struct renderResource*, Resource*);
    
    //load function, load image and parameters.
    void (*load)(struct renderResource*, uint16_t, uint16_t, uint8_t*);
    //unload function, set the image pointer to NULL.
    void (*unload)(struct renderResource*);
}RenderResource;

//Constructor for RenderResource
void renderResourceNew(RenderResource*, Resource*);
void renderResourceLoad(RenderResource*, uint16_t, uint16_t, uint8_t*);
void renderResourceUnload(RenderResource*);

/*
    RenderObject class, includes a RenderResource and parameters for rendering.
*/
typedef struct renderObject
{
    RenderResource* mRenderResource;

    int16_t mPosX;
    int16_t mPosY;
    uint8_t mVisible;

    //next and previous RenderObject
    struct renderObject* nextObj;
    struct renderObject* prevObj;

    //constructor of RenderObject
    void (*new)(struct renderObject*, RenderResource*, int16_t, int16_t, uint8_t);
    
}RenderObject;

//constructor of RenderObject
void renderObjectNew(RenderObject*, RenderResource*, int16_t, int16_t, uint8_t);

/*
    RenderManager class, manages RenderResource and RenderObject objects.
*/
typedef struct renderManager
{
    uint8_t mRenderResourceCount;
    uint16_t mRenderObjectCount;
    void (*new)(struct renderManager*);

    RenderResource mRenderResources[MAX_OBJECT_COUNT];
    RenderObject mInitRenderObject; //a sentinel of the linked list

    //Finds RenderResource by its ID, returns NULL if not found.
    RenderResource* (*findRenderResourceByID)(struct renderManager*, uint8_t);
    //Finds RenderResource by its name, returns NULL if not found.
    RenderResource* (*findRenderResourceByName)(struct renderManager*, char[]); 

    //Adds RenderResource, whose base class is also registered in the ResourceManager.
    void (*addResource)(struct renderManager*, ResourceManager*, char[], uint16_t, uint16_t, uint8_t*);

    //Adds RenderObject to the mRenderObjects, which is to be rendered.
    void (*addObject)(struct renderManager*, RenderObject*);
    //Removes RenderObject in the list.
    void (*removeObject)(struct renderManager*, RenderObject*);

    //Clear the current render list.
    void (*clear)(struct renderManager*);
    //Render all objects in the linked list (IN ORDER).
    void (*renderAllObject)(struct renderManager*);
}RenderManager;

//constructor of RenderManager
void renderManagerNew(RenderManager*);
RenderResource* renderManagerFindRenderResourceByID(RenderManager*, uint8_t); 
RenderResource* renderManagerFindRenderResourceByName(RenderManager*, char[]); 
void renderManagerAddResource(RenderManager*, ResourceManager*, char[], uint16_t, uint16_t, uint8_t*);
void renderManagerAddObject(RenderManager*, RenderObject*);   
void renderManagerRemoveObject(RenderManager*, RenderObject*); 
void renderManagerClear(RenderManager*);
void renderManagerRenderAllObject(RenderManager*);

#endif /* _RENDERMANAGER_H_ */