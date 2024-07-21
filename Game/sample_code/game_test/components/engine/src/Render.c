/*
    Render.c

    Functions of Render.h.
*/

#ifndef _RENDER_C_
#define _RENDER_C_

#include <string.h>

#include <math.h>
#include "../include/Resource.h"
#include "../include/Render.h"

//include library "st7789"
#include "../../st7789/include/st7789.h"
#include "../../fontx/include/fontx.h"
#include "../../pngle/include/pngle.h"
#include "../../decode_png/include/decode_png.h"

/*
    RenderResource class
*/
void renderResourceNew(RenderResource* obj, uint8_t ID, Resource* resource, uint16_t width, uint16_t height)
{
    obj->new = renderResourceNew;
    obj->mRenderResourceID = ID;
    obj->mWidth = width;
    obj->mHeight = height;
    obj->setLoadFunction = renderResourceSetLoadFunction;
    obj->loadImage = renderResourceLoadImage;
    //obj->loadFont = renderResourceLoadFont;
    obj->setUnloadFunction = renderResourceSetUnloadFunction;
    obj->unloadImage = renderResourceUnloadImageDefault;
    obj->baseResource = resource;
    return;
}

void renderResourceSetLoadFunction(RenderResource* obj, void (*function)(struct renderResource*, const uint16_t*))
{
    obj->loadImage = function;
    obj->setUnloadFunction(obj, renderResourceUnloadImageHeapFree);
}

//Template for loading functions
void renderResourceLoadImage(RenderResource* obj, const uint16_t* image)
{
    obj->mImage = image;
}

/*
void renderResourceLoadFont(RenderResource* obj)
{
    if(obj->baseResource->mType == RESOURCE_TEXT)
    {
        InitFontx(obj->mFont, obj->baseResource->mFileName, "");
    }
    return;
}
*/

void renderResourceSetUnloadFunction(RenderResource* obj, void (*function)(struct renderResource*))
{
    obj->unloadImage = function;
}

void renderResourceUnloadImageDefault(RenderResource* obj)
{
    if(obj->mImage != NULL) obj->mImage = NULL;
    return;
}

void renderResourceUnloadImageHeapFree(RenderResource* obj)
{
    if(obj->mImage != NULL) free(obj->mImage);
    return;
}

/*
    RenderObject class
*/
void renderObjectNew(RenderObject* obj, RenderResource* renderResource, int16_t posX, int16_t posY, uint8_t visible)
{
    obj->mRenderResource = renderResource;
    obj->mPrePosX = posX;
    obj->mPrePosY = posY;
    obj->mPosX = posX;
    obj->mPosY = posY;
    obj->mColor = WHITE;
    obj->mPreVisible = visible;
    obj->mVisible = visible;
    //obj->mScale = scale;
    obj->new = renderObjectNew;
    obj->getPosX = renderObjectGetPosX;
    obj->getPosY = renderObjectGetPosY;
    obj->getVisible = renderObjectGetVisible;
    obj->setImage = renderObjectSetImage;
    obj->setPos = renderObjectSetPos;
    obj->setColor = renderObjectSetColor;
    obj->setVisible = renderObjectSetVisible;
    //obj->setText = renderObjectSetText;
    obj->add = renderObjectAdd;
    obj->remove = renderObjectRemove;
    obj->render = renderObjectRender;
    return;
}

int16_t renderObjectGetPosX(RenderObject* obj)
{
    return obj->mPosX;
}

int16_t renderObjectGetPosY(RenderObject* obj)
{
    return obj->mPosY;
}

uint8_t renderObjectGetVisible(RenderObject* obj)
{
    return obj->mVisible;
}

void renderObjectSetImage(RenderObject* obj, RenderResource* renderResource)
{
    obj->mRenderResource = renderResource;
    return;
}

void renderObjectSetPos(RenderObject* obj, int16_t posX, int16_t posY)
{
    obj->mPrePosX = obj->mPosX;
    obj->mPrePosY = obj->mPosY;
    obj->mPosX = posX;
    obj->mPosY = posY;
    return;
}

void renderObjectSetColor(RenderObject* obj, uint16_t color)
{
    obj->mPrePosX = obj->mPosX;
    obj->mPrePosY = obj->mPosY;
    obj->mColor = color;
    return;
}

void renderObjectSetVisible(RenderObject* obj, uint8_t visible)
{
    obj->mPreVisible = obj->mVisible;
    obj->mVisible = visible;
    return;
}

/*
void renderObjectSetText(RenderObject* obj, char text[])
{
    obj->mPrePosX = obj->mPosX;
    obj->mPrePosY = obj->mPosY;    
    if(obj->mRenderResource->baseResource->mType == RESOURCE_TEXT) strcpy((char*)(obj->mText), text);
    return;
}
*/

void renderObjectAdd(RenderObject* obj, RenderManager* renderManager)
{
    renderManager->addObject(renderManager, obj);
    return;
}

void renderObjectRemove(RenderObject* obj, RenderManager* renderManager)
{
    renderManager->removeObject(renderManager, obj);
    return;
}

void renderObjectRender(RenderObject* obj, RenderManager* renderManager)
{
    renderManager->reRender(renderManager, obj);
    return;
}

/*
    RenderManager class
*/
RenderManager* newRenderManager()
{
    RenderManager* renderManager = calloc(1, sizeof(RenderManager));
    renderManagerNew(renderManager);
    return renderManager;
}

void deleteRenderManager(RenderManager* obj)
{
    if(obj != NULL) free(obj);
}

void renderManagerNew(RenderManager* obj)
{
    spi_master_init(&(obj->TFT_t), CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
    lcdInit(&(obj->TFT_t), CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);
    lcdFillScreen(&(obj->TFT_t), BLACK);
    
    obj->new = renderManagerNew;

    obj->mRenderResourceCount = 0;
    obj->mRenderObjectCount = 0;

    obj->mInitRenderObject.mRenderResource = NULL;
    obj->mInitRenderObject.nextObj = &(obj->mInitRenderObject);
    obj->mInitRenderObject.prevObj = &(obj->mInitRenderObject);

    obj->findRenderResourceByID = renderManagerFindRenderResourceByID;
    obj->findRenderResourceByName = renderManagerFindRenderResourceByName;
    obj->addImage = renderManagerAddImage;
    obj->addImageByFunction = renderManagerAddImageByFunction;
    //obj->addFont = renderManagerAddFont;
    obj->addObject = renderManagerAddObject;
    obj->removeObject = renderManagerRemoveObject;
    obj->copy = renderManagerCopy;
    obj->readDown = renderManagerReadDown;
    obj->readUp = renderManagerReadUp;
    obj->readFull = renderManagerReadFull;
    obj->clear = renderManagerClear;
    obj->reRender = renderManagerReRender;
    obj->update = renderManagerUpdate;
    //obj->renderAllText = renderManagerRenderAllText;
    //obj->renderAllObject = renderManagerRenderAllObject;
    return;
}

RenderResource* renderManagerFindRenderResourceByID(RenderManager* obj, uint8_t ID)
{
    if(ID < obj->mRenderResourceCount)
    {
        return &(obj->mRenderResources[ID]);
    }
    else return NULL;
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

void renderManagerAddImage(RenderManager* obj, ResourceManager* resourceManager, char name[], const uint16_t* image, const uint16_t width, const uint16_t height)
{
    RenderResource* existFile = obj->findRenderResourceByName(obj, name);
    if(existFile != NULL)
    {
        existFile->loadImage(existFile, image);
    }
    else
    {
        resourceManager->addResource(resourceManager, name, RESOURCE_GRAPHIC);

        renderResourceNew(&obj->mRenderResources[obj->mRenderResourceCount], obj->mRenderResourceCount, resourceManager->findResourceByID(resourceManager, (resourceManager->mResourceCount) - 1), width, height);
        obj->mRenderResources[obj->mRenderResourceCount].loadImage(&obj->mRenderResources[obj->mRenderResourceCount], image);

        obj->mRenderResourceCount++;
    }
    return;
}

void renderManagerAddImageByFunction(RenderManager* obj, ResourceManager* resourceManager, char name[], void (*function)(RenderResource*, uint16_t*), uint16_t width, uint16_t height)
{
    resourceManager->addResource(resourceManager, name, RESOURCE_GRAPHIC);

    renderResourceNew(&obj->mRenderResources[obj->mRenderResourceCount], obj->mRenderResourceCount, resourceManager->findResourceByID(resourceManager, (resourceManager->mResourceCount) - 1), width, height);
    obj->mRenderResources[obj->mRenderResourceCount].setLoadFunction(&obj->mRenderResources[obj->mRenderResourceCount], function);
    obj->mRenderResources[obj->mRenderResourceCount].setUnloadFunction(&obj->mRenderResources[obj->mRenderResourceCount], renderResourceUnloadImageHeapFree);
    obj->mRenderResources[obj->mRenderResourceCount].loadImage(&obj->mRenderResources[obj->mRenderResourceCount], NULL);

    obj->mRenderResourceCount++;
    return;
}

/*
void renderManagerAddFont(RenderManager* obj, ResourceManager* resourceManager, char name[])
{
    resourceManager->addResource(resourceManager, name, RESOURCE_TEXT);
    RenderResource newRenderResource;
    renderResourceNew(&newRenderResource, obj->mRenderResourceCount, resourceManager->findResourceByID(resourceManager, (resourceManager->mResourceCount) - 1), 0, 0);
    renderResourceLoadFont(&newRenderResource);

    obj->mRenderResources[obj->mRenderResourceCount] = newRenderResource;
    obj->mRenderResourceCount++;
    return;
}
*/

void renderManagerAddObject(RenderManager* obj, RenderObject* renderObject)
{
    renderObject->prevObj = obj->mInitRenderObject.prevObj;
    (obj->mInitRenderObject.prevObj)->nextObj = renderObject;
    renderObject->nextObj = &(obj->mInitRenderObject);
    obj->mInitRenderObject.prevObj = renderObject;
    obj->reRender(obj, renderObject);
    return;
}

void renderManagerRemoveObject(RenderManager* obj, RenderObject* renderObject)
{
    uint8_t tempVisible = renderObject->mVisible;
    renderObject->setVisible(renderObject, 0);
    obj->reRender(obj, renderObject);
    renderObject->setVisible(renderObject, tempVisible);
    if(renderObject->prevObj != NULL) renderObject->prevObj->nextObj = renderObject->nextObj;
    if(renderObject->nextObj != NULL) renderObject->nextObj->prevObj = renderObject->prevObj;
    renderObject->prevObj = NULL;
    renderObject->nextObj = NULL;
}

void renderManagerCopy(RenderManager* obj, RenderObject* renderObject, int16_t posX, int16_t posY, uint16_t width, uint16_t height)
{
    if(renderObject->mRenderResource->baseResource->mType == RESOURCE_GRAPHIC)
    {
        for(uint16_t i = (renderObject->mPosX >= posX ? 0 : posX - renderObject->mPosX); i < (renderObject->mRenderResource->mWidth); i++)
        {
            if(renderObject->mPosX + i >= width + posX) break;
            for(uint16_t j = (renderObject->mPosY >= posY ? 0 : posY - renderObject->mPosY); j < (renderObject->mRenderResource->mHeight); j++)
            {
                if(renderObject->mPosY + j >= height + posY) break;
                if(renderObject->mRenderResource->mImage[i + j * renderObject->mRenderResource->mWidth] != TRANSPARENT)
                {
                    obj->mScreen[(i + renderObject->mPosX - posX) + (j + renderObject->mPosY - posY) * width] = 
                        renderObject->mRenderResource->mImage[i + j * renderObject->mRenderResource->mWidth];
                }
            }
        }
    }
    return;
}

void renderManagerReadDown(RenderManager* obj, RenderObject* renderObject)
{
    obj->mScreen = calloc(renderObject->mRenderResource->mWidth*renderObject->mRenderResource->mHeight, sizeof(uint16_t));
    RenderObject currentNode = obj->mInitRenderObject;
    while(currentNode.nextObj->mRenderResource != NULL)
    {
        if(currentNode.nextObj->mVisible == 1)
        {
            obj->copy(obj, currentNode.nextObj, renderObject->mPrePosX, renderObject->mPrePosY, renderObject->mRenderResource->mWidth, renderObject->mRenderResource->mHeight);
        }
        currentNode = *(currentNode.nextObj);
    }
    lcdDrawPNG(&(obj->TFT_t), renderObject->mPrePosX, renderObject->mPrePosY, obj->mScreen, renderObject->mRenderResource->mWidth, renderObject->mRenderResource->mHeight);
    free(obj->mScreen);
    return;
}

void renderManagerReadUp(RenderManager* obj, RenderObject* renderObject)
{
    obj->mScreen = calloc(renderObject->mRenderResource->mWidth*renderObject->mRenderResource->mHeight, sizeof(uint16_t));
    RenderObject currentNode = obj->mInitRenderObject;
    while(currentNode.nextObj->mRenderResource != NULL)
    {
        if(currentNode.nextObj->mVisible == 1)
        {
            obj->copy(obj, currentNode.nextObj, renderObject->mPosX, renderObject->mPosY, renderObject->mRenderResource->mWidth, renderObject->mRenderResource->mHeight);
        }
        currentNode = *(currentNode.nextObj);
    }
    lcdDrawPNG(&(obj->TFT_t), renderObject->mPosX, renderObject->mPosY, obj->mScreen, renderObject->mRenderResource->mWidth, renderObject->mRenderResource->mHeight);
    free(obj->mScreen);
    return;
}

void renderManagerReadFull(RenderManager* obj, RenderObject* renderObject)
{
    int16_t fullPosX = renderObject->mPrePosX >= renderObject->mPosX ? renderObject->mPosX : renderObject->mPrePosX;
    int16_t fullWidth = renderObject->mPrePosX + renderObject->mPosX - 2 * fullPosX + renderObject->mRenderResource->mWidth;
    int16_t fullPosY = renderObject->mPrePosY >= renderObject->mPosY ? renderObject->mPosY : renderObject->mPrePosY;
    int16_t fullHeight = renderObject->mPrePosY + renderObject->mPosY - 2 * fullPosY + renderObject->mRenderResource->mHeight;
    
    RenderObject currentNode = obj->mInitRenderObject;
    if(fullWidth * fullHeight > (SCREEN_WIDTH * SCREEN_HEIGHT / 2))
    {
        if(fullHeight % 2 == 0)
        {
            obj->mScreen = calloc(fullWidth * fullHeight / 2, sizeof(uint16_t));
            while(currentNode.nextObj->mRenderResource != NULL)
            {
                if(currentNode.nextObj->mVisible == 1)
                {
                    obj->copy(obj, currentNode.nextObj, fullPosX, fullPosY, fullWidth, fullHeight / 2);
                }
                currentNode = *(currentNode.nextObj);
            }
            lcdDrawPNG(&(obj->TFT_t), fullPosX, fullPosY, obj->mScreen, fullWidth, fullHeight / 2);
            free(obj->mScreen);
            obj->mScreen = calloc(fullWidth * fullHeight / 2, sizeof(uint16_t));
            currentNode = obj->mInitRenderObject;
            while(currentNode.nextObj->mRenderResource != NULL)
            {
                if(currentNode.nextObj->mVisible == 1)
                {
                    obj->copy(obj, currentNode.nextObj, fullPosX, fullPosY + fullHeight / 2, fullWidth, fullHeight / 2);
                }
                currentNode = *(currentNode.nextObj);
            }
            lcdDrawPNG(&(obj->TFT_t), fullPosX, fullPosY + fullHeight / 2, obj->mScreen, fullWidth, fullHeight / 2);
            free(obj->mScreen);
        }
        else
        {
            obj->mScreen = calloc(fullWidth * (fullHeight / 2), sizeof(uint16_t));
            while(currentNode.nextObj->mRenderResource != NULL)
            {
                if(currentNode.nextObj->mVisible == 1)
                {
                    obj->copy(obj, currentNode.nextObj, fullPosX, fullPosY, fullWidth, fullHeight / 2);
                }
                currentNode = *(currentNode.nextObj);
            }
            lcdDrawPNG(&(obj->TFT_t), fullPosX, fullPosY, obj->mScreen, fullWidth, fullHeight / 2);
            free(obj->mScreen);
            obj->mScreen = calloc(fullWidth * (fullHeight / 2 + 1), sizeof(uint16_t));
            currentNode = obj->mInitRenderObject;
            while(currentNode.nextObj->mRenderResource != NULL)
            {
                if(currentNode.nextObj->mVisible == 1)
                {
                    obj->copy(obj, currentNode.nextObj, fullPosX, fullPosY + fullHeight / 2, fullWidth, fullHeight / 2 + 1);
                }
                currentNode = *(currentNode.nextObj);
            }
            lcdDrawPNG(&(obj->TFT_t), fullPosX, fullPosY + fullHeight / 2, obj->mScreen, fullWidth, fullHeight / 2 + 1);
            free(obj->mScreen);
        }
    }
    else
    {
        obj->mScreen = calloc(fullWidth * fullHeight, sizeof(uint16_t));
        while(currentNode.nextObj->mRenderResource != NULL)
        {
            if(currentNode.nextObj->mVisible == 1)
            {
                obj->copy(obj, currentNode.nextObj, fullPosX, fullPosY, fullWidth, fullHeight);
            }
            currentNode = *(currentNode.nextObj);
        }
        lcdDrawPNG(&(obj->TFT_t), fullPosX, fullPosY, obj->mScreen, fullWidth, fullHeight);
        free(obj->mScreen);
    }
    return;
}

void renderManagerClear(RenderManager* obj)
{
    while(obj->mInitRenderObject.nextObj->mRenderResource != NULL)
    {
        obj->removeObject(obj, obj->mInitRenderObject.nextObj);
    }
    return;
}

void renderManagerReRender(RenderManager* obj, RenderObject* renderObject)
{
    if(renderObject->mVisible == 1 || renderObject->mPreVisible != renderObject->mVisible)
    {
        if(abs(renderObject->mPosX - renderObject->mPrePosX) < renderObject->mRenderResource->mWidth
            && abs(renderObject->mPosY - renderObject->mPrePosY) < renderObject->mRenderResource->mHeight)
        {
            obj->readFull(obj, renderObject);
        }
        else
        {
            uint8_t tempVisible = renderObject->mVisible;
            renderObject->setVisible(renderObject, 0);
            obj->readDown(obj, renderObject);
            renderObject->setVisible(renderObject, tempVisible);
            obj->readUp(obj, renderObject);
        }
    }

    renderObject->mPrePosX = renderObject->mPosX;
    renderObject->mPrePosY = renderObject->mPosY;
    renderObject->mPreVisible = renderObject->mVisible;
    return;
}

void renderManagerUpdate(RenderManager* obj)
{
    lcdDrawFinish(&(obj->TFT_t));
    return;
}

/*
void renderManagerRenderAllText(RenderManager* obj)
{
    RenderObject currentNode = obj->mInitRenderObject;
    while(currentNode.nextObj->mRenderResource != NULL)
    {
        currentNode = *(currentNode.nextObj);
        if(currentNode.mRenderResource->baseResource->mType == RESOURCE_TEXT) currentNode.render(&currentNode, &(obj->TFT_t));
    }
    return;
}
*/

/*
void renderManagerRenderAllObject(RenderManager* obj)
{
    RenderObject currentNode = obj->mInitRenderObject;
    while(currentNode.nextObj->mRenderResource != NULL)
    {
        printf("%s", "Rendered an object, named ");
        printf("%s\n", currentNode.nextObj->mRenderResource->baseResource->mFileName);
        currentNode = *(currentNode.nextObj);
        currentNode.render(&currentNode, &(obj->TFT_t));
    }
    printf("%s\n", "Finish rendering.");
    return;
}
*/

#endif /* _RENDER_C_ */