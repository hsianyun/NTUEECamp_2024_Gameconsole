/*
    Resource.c

    Functions of Resource.h.
*/

#ifndef _RESOURCE_C_
#define _RESOURCE_C_

#include <string.h>
#include "../include/Resource.h"

/*
    Resource class
*/
void resourceNew(Resource* obj, uint8_t ID, char name[], RESOURCE_TYPE type)
{
    obj->new = resourceNew;
    obj->mResourceID = ID;
    obj->mType = type;
    strcpy(obj->mFileName, name);
    return;
}

/*
    ResourceManager class
*/
ResourceManager* newResourceManager()
{
    ResourceManager* resourceManager = calloc(1, sizeof(ResourceManager));
    resourceManagerNew(resourceManager);
    return resourceManager;
}

void deleteResourceManager(ResourceManager* obj)
{
    if(obj != NULL) free(obj);
}

void resourceManagerNew(ResourceManager* obj)
{
    obj->new = resourceManagerNew;
    obj->mResourceCount = 0;
    obj->findResourceByID = resourceManagerFindResourceByID;
    obj->findResourceByName = resourceManagerFindResourceByName;
    obj->addResource = resourceManagerAddResource;
    return;
}

Resource* resourceManagerFindResourceByID(ResourceManager* obj, uint8_t ID)
{
    if(ID < obj->mResourceCount)
    {
        return &(obj->mResources[ID]);
    }
    else return NULL;
}

Resource* resourceManagerFindResourceByName(ResourceManager* obj, char name[])
{
    for(uint8_t i = 0; i < obj->mResourceCount; i++)
    {
        if(strcmp(obj->mResources[i].mFileName, name) == 0)
        {
            return &(obj->mResources[i]);
        }
    }
    return NULL;
}

void resourceManagerAddResource(ResourceManager* obj, char name[], RESOURCE_TYPE type)
{
    Resource newResource;
    resourceNew(&newResource, obj->mResourceCount, name, type);
    obj->mResources[obj->mResourceCount] = newResource;
    obj->mResourceCount++;
    return;
}

#endif /* _RESOURCE_C_ */