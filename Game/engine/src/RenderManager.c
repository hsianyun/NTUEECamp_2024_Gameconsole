/*
    ResourceManager.c

    Functions of RenderManager.h.
*/

#ifndef _RENDERMANAGER_C_
#define _RENDERMANAGER_C_

#include <string.h>
#include <stdio.h>
#include "../include/ResourceManager.h"
#include "../include/RenderManager.h"
#define MAX_OBJECT_COUNT 200

/*
    RenderResource class
*/
void renderResourceNew(RenderResource* obj, Resource* resource)
{
    obj->new = renderResourceNew;
    obj->load = renderResourceLoad;
    obj->unload = renderResourceUnload;
    obj->baseResource = resource;
    return;
}

void renderResourceLoad(RenderResource* obj, uint16_t width, uint16_t height, uint8_t* image)
{
    obj->mWidth = width;
    obj->mHeight = height;
    obj->mImage = image;
    return;
}

void renderResourceUnload(RenderResource* obj)
{
    if(obj->mImage != NULL) obj->mImage = NULL;
    return;
}

/*
    RenderObject class
*/
void renderObjectNew(RenderObject* obj, RenderResource* renderResource, int16_t posX, int16_t posY, uint8_t visible)
{
    obj->mRenderResource = renderResource;
    obj->mPosX = posX;
    obj->mPosY = posY;
    obj->mVisible = visible;
    obj->new = renderObjectNew;
    return;
}

/*
    RenderManager class
*/
void renderManagerNew(RenderManager* obj)
{
    obj->new = renderManagerNew;
    obj->mRenderResourceCount = 0;
    obj->mRenderObjectCount = 0;

    obj->mInitRenderObject.mRenderResource = NULL;
    obj->mInitRenderObject.nextObj = &(obj->mInitRenderObject);
    obj->mInitRenderObject.prevObj = &(obj->mInitRenderObject);

    obj->findRenderResourceByID = renderManagerFindRenderResourceByID;
    obj->findRenderResourceByName = renderManagerFindRenderResourceByName;
    obj->addResource = renderManagerAddResource;
    obj->addObject = renderManagerAddObject;
    obj->removeObject = renderManagerRemoveObject;
    obj->clear = renderManagerClear;
    obj->renderAllObject = renderManagerRenderAllObject;
    return;
}

RenderResource* renderManagerFindRenderResourceByID(RenderManager* obj, uint8_t ID)
{
    for(uint8_t i = 0; i < obj->mRenderResourceCount; i++)
    {
        if(obj->mRenderResources[i].baseResource->mResourceID == ID)
        {
            return &(obj->mRenderResources[i]);
        }
    }
    return NULL;
} 

RenderResource* renderManagerFindRenderResourceByName(RenderManager* obj, char name[])
{
    for(uint8_t i = 0; i < obj->mRenderResourceCount; i++)
    {
        if(strcmp(((obj->mRenderResources[i]).baseResource)->mFileName, name) == 0)
        {
            return &(obj->mRenderResources[i]);
        }
    }
    return NULL;
}

void renderManagerAddResource(RenderManager* obj, ResourceManager* resourceManager, char name[], uint16_t width, uint16_t height, uint8_t* image)
{
    resourceManager->addResource(resourceManager, name, RESOURCE_GRAPHIC);
    RenderResource newRenderResource;
    renderResourceNew(&newRenderResource, resourceManager->findResourceByID(resourceManager, (resourceManager->mResourceCount) - 1));
    renderResourceLoad(&newRenderResource, width, height, image);

    obj->mRenderResources[obj->mRenderResourceCount] = newRenderResource;
    obj->mRenderResourceCount++;
    return;
}

void renderManagerAddObject(RenderManager* obj, RenderObject* renderObject)
{
    renderObject->prevObj = obj->mInitRenderObject.prevObj;
    (obj->mInitRenderObject.prevObj)->nextObj = renderObject;
    renderObject->nextObj = &(obj->mInitRenderObject);
    obj->mInitRenderObject.prevObj = renderObject;
}

void renderManagerRemoveObject(RenderManager* obj, RenderObject* renderObject)
{
    renderObject->prevObj->nextObj = renderObject->nextObj;
    renderObject->nextObj->prevObj = renderObject->prevObj;
    renderObject->prevObj = NULL;
    renderObject->nextObj = NULL;
    printf("%s", "Revome an object, named ");
    printf("%s\n", renderObject->mRenderResource->baseResource->mFileName);
}

void renderManagerClear(RenderManager* obj)
{
    while(obj->mInitRenderObject.nextObj->mRenderResource != NULL)
    {
        obj->removeObject(obj, obj->mInitRenderObject.nextObj);
    }
    printf("%s\n", "Finish clearing.");
    return;
}

void renderManagerRenderAllObject(RenderManager* obj)
{
    RenderObject currentNode = obj->mInitRenderObject;
    while(currentNode.nextObj->mRenderResource != NULL)
    {
        printf("%s", "Rendered an object, named ");
        printf("%s\n", currentNode.nextObj->mRenderResource->baseResource->mFileName);
        currentNode = *(currentNode.nextObj);
    }
    printf("%s\n", "Finish rendering.");
    return;
}

#endif /* _RENDERMANAGER_C_ */