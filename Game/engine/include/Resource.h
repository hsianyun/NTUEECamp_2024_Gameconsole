/*
    Resource.h

    Class Resource and Resource.
*/

#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <stdint.h>
#define MAX_OBJECT_COUNT 20
#define MAX_FILENAME_LENGTH 64

//Enum to indicate resource type
typedef enum{
    RESOURCE_NULL = 0,
    RESOURCE_GRAPHIC = 1,
    RESOURCE_TEXT = 2,
    RESOURCE_AUDIO = 3,
}RESOURCE_TYPE; 

/*
    Resource class, indicate a resource object.
*/
typedef struct resource
{
    uint8_t mResourceID;
    char mFileName[MAX_FILENAME_LENGTH];
    RESOURCE_TYPE mType;

    //Constructor for Resource
    void (*new)(struct resource*, uint8_t, char[], RESOURCE_TYPE);
    /*
        The following function is required for each derived class.:
        void (*load)(TYPE*);
        void (*unload)(TYPE*);
    */
}Resource;

//Constructor for Resource
void resourceNew(Resource*, uint8_t, char[], RESOURCE_TYPE);

/*
    ResourceManager class, manages resource objects.
*/
typedef struct resourceManager
{
    uint8_t mResourceCount;
    void (*new)(struct resourceManager*);

    Resource mResources[MAX_OBJECT_COUNT];

    //Finds resource by its ID, returns NULL if not found.
    Resource* (*findResourceByID)(struct resourceManager*, uint8_t); 
    //Finds resource by its name, returns NULL if not found.
    Resource* (*findResourceByName)(struct resourceManager*, char[]);

    //Adds resource.
    void (*addResource)(struct resourceManager*, char[], RESOURCE_TYPE);
}ResourceManager;

ResourceManager* newResourceManager();
void deleteResourceManager(ResourceManager*);
//Constructor for ResourceManager
void resourceManagerNew(ResourceManager*);
Resource* resourceManagerFindResourceByID(ResourceManager*, uint8_t);
Resource* resourceManagerFindResourceByName(ResourceManager*, char[]); 
void resourceManagerAddResource(ResourceManager*, char[], RESOURCE_TYPE);

#endif /* _RESOURCE_H_ */