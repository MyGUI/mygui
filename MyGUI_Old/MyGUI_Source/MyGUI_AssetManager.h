#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_Skin.h"
#include "MyGUI_LoadINI.h"

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
	private:
        __AssetDefinition<__tag_MYGUI_SKIN_INFO >     mSkins;
        __AssetDefinition<__tag_MYGUI_FONT_INFO >       mFonts;
        __AssetDefinition<__tag_MYGUI_POINTER_INFO >    mPointers;
        __AssetDefinition<__MYGUI_SUBSKIN_INFO >       mSubSkins;
        
        void loadDefinitions(const String & strFileName);

        void loadSkinDefinitions(loadINI & ini);
		void loadSubSkinDefinitions(loadINI & ini, const String & Name, __tag_MYGUI_SKIN_INFO * window, unsigned int & UniqueSerialID);
		void loadPointerDefinitions(loadINI & ini);
		void loadFontDefinitions(loadINI & ini);

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
        
        __AssetDefinition<__MYGUI_SUBSKIN_INFO> *SubSkins()
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

	public:

		// возвращает список путей к файлам по маске
		static void getResourcePath(std::vector<String> & saFilePath, const String & strMaskFileName);
		// возвращает размер текстуры материала
		static bool getMaterialSize(const String & materialName, uint16 & sizeX, uint16 & sizeY);


		// константы для парсинга
		static const String BLOCK_WINDOW_NAME;
		static const String VALUE_WINDOW_SKIN;
		static const String VALUE_WINDOW_FONT;
		static const String VALUE_WINDOW_COLOUR;
		static const String VALUE_WINDOW_DATA1;
		static const String VALUE_WINDOW_DATA2;
		static const String VALUE_WINDOW_DATA3;
		static const String VALUE_WINDOW_DATA4;

		static const String VALUE_WINDOW_ADDED_SKIN1;
		static const String VALUE_WINDOW_ADDED_SKIN2;
		static const String VALUE_WINDOW_ADDED_DATA1;
		static const String VALUE_WINDOW_ADDED_DATA2;
		static const String VALUE_WINDOW_MATERIAL;

		static const String BLOCK_FONT_NAME;
		static const String VALUE_FONT_NAME;
		static const String VALUE_FONT_SIZE;

		static const String BLOCK_POINTER_NAME;
		static const String VALUE_POINTER_SIZE;
		static const String VALUE_POINTER_OFFSET;
		static const String VALUE_POINTER_SKIN;

		static const String SECTION_SUB_SKIN;
		static const String SECTION_SUB_SKIN_MAIN;

		static const String VALUE_SKIN_POSITION;
		static const String VALUE_SKIN_ALIGN;
		static const String VALUE_SKIN_EVENT;
		static const String VALUE_SKIN_STYLE;
		static const String VALUE_SKIN_OFFSET;

		static const String VALUE_SKIN_DEACTIVATED;
		static const String VALUE_SKIN_NORMAL;
		static const String VALUE_SKIN_ACTIVED;
		static const String VALUE_SKIN_PRESSED;
		static const String VALUE_SKIN_SELECTED;

		static const String VALUE_OFFSET_DEACTIVATED;
		static const String VALUE_OFFSET_NORMAL;
		static const String VALUE_OFFSET_ACTIVED;
		static const String VALUE_OFFSET_PRESSED;
		static const String VALUE_OFFSET_SELECTED;

		// карты имен для парсинга
		//Ripe for movement to a managed asset in the asset manager
        //Maps enums to strings and vice versa for use in scripts
		typedef std::map<String, uint16> mapValue;
		typedef mapValue::iterator MapValueIterator;

		static mapValue mMapValueEvent;
		static mapValue mMapValueAligin;
		static mapValue mMapValueStyle;

	}; // class AssetManager
} // namespace MyGUI