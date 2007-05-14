#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_Skin.h"

namespace MyGUI
{
    template<typename Type> class __AssetDefinition
    { 
    protected:
        std::map<String, typename Type *> mDefinitions;
        
        Type *addNewDefinition(const String &Name)
        {
            return (mDefinitions[Name] = new Type);
        }
        
        Type *__addNewDefinition(const String &Name, const Type *type)
        {
            mDefinitions.insert(Name, type);
            return type;
        }
        
    public:
        typedef typename std::map<String, Type *>::iterator iterator;
        
        Type *defineNew(const String &Name)
        {
            if(getDefinition(Name))
                return getDefinition(Name);
            return addNewDefinition(Name);
        }
        
        Type *__defineNew(const String &Name, const Type &type)
        {
            if(getDefinition(Name))
            {
                ASSERT(!"Attempting to define a new definition, but the "
                        "name is already associated with a definition");
                return NULL; //ambiguous possible error behavior
            }
            return __addNewDefinition(Name, new Type(type));
        }
        
        void deleteDefinition(const String &Name)
        {
            if(getDefinition(Name) != NULL)
            {
                Type *deleteme = getDefinition(Name);
                delete deleteme;
                mDefinitions.erase(Name);
            }
        }
        
        void clearAll()
        {
            while(!mDefinitions.empty())
                deleteDefinition(begin()->first);
        }
        
        Type *getDefinition(const String &Name)
        {
            if(mDefinitions.find(Name) != mDefinitions.end())
                return mDefinitions[Name];
            return 0; //definition not found
        }
        
        iterator begin()
        {
            return mDefinitions.begin();
        }
        
        iterator end()
        {
            return mDefinitions.end();
        }
    };
    
    class AssetManager
    {
        __AssetDefinition<__tag_MYGUI_SKIN_INFO >     mSkins;
        __AssetDefinition<__tag_MYGUI_FONT_INFO >       mFonts;
        __AssetDefinition<__tag_MYGUI_POINTER_INFO >    mPointers;
        __AssetDefinition<__tag_MYGUI_SUBSKIN_INFO >       mSubSkins;
        
        void loadSkinDefinitions(std::map<String, uint32> & mapNameValue, const String & strFileName);
        
    public:
        typedef __AssetDefinition<__tag_MYGUI_SKIN_INFO>::iterator   SkinIterator;
        typedef __AssetDefinition<__tag_MYGUI_FONT_INFO>::iterator     FontIterator;
        typedef __AssetDefinition<__tag_MYGUI_POINTER_INFO>::iterator  PointerIterator;
        
        static AssetManager *getSingleton()
        {
            static AssetManager Instance;
            return &Instance;
        }
        
        __AssetDefinition<__tag_MYGUI_SKIN_INFO> *Skins()
        {
            return &mSkins;
        }
        
        __AssetDefinition<__tag_MYGUI_FONT_INFO> *Fonts()
        {
            return &mFonts;
        }
        
        __AssetDefinition<__tag_MYGUI_POINTER_INFO> *Pointers()
        {
            return &mPointers;
        }
        
        __AssetDefinition<__tag_MYGUI_SUBSKIN_INFO> *SubSkins()
        {
            return &mSubSkins;
        }
        
        /*
            Loads the skin definitions from any files in the
            Ogre resource paths.  This sets up skin definitions,
            fonts, and mouse pointers.
        
            RETURNS: a reference to the AssetManager to allow for
            chaining.
        */
        AssetManager *loadAssets();
        
        /*
            Unloads the skin definitions from MyGUI's core.
        
            RETURNS: a reference to the AssetManager to allow for
            chaining.
        */
        AssetManager *unloadAssets();
    };   
}