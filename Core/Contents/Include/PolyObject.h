/*
 Copyright (C) 2011 by Ivan Safrin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#pragma once
#include "PolyGlobals.h"
#include "PolyString.h"

class TiXmlElement;

namespace Polycode {

	/**
	* Single entry in an Object. Object entries can be accessed as dictionaries or arrays.
	*/
	class _PolyExport ObjectEntry {
	public:
		
		/**
		* Default constructor
		*/
		ObjectEntry() { type = ObjectEntry::CONTAINER_ENTRY; }
				
		/**
		* Type of entry. Possible values are (FLOAT_ENTRY, INT_ENTRY, BOOL_ENTRY, ARRAY_ENTRY, STRING_ENTRY, CONTAINER_ENTRY).
		*/
		int type;
		
		/**
		* Lookup key for this object entry.
		*/		
		String name;

		/**
		* Number value of this object entry.
		*/				
		Number NumberVal;
		
		/**
		* Integer value of this object entry.
		*/						
		int intVal;
		
		/**
		* String value of this object entry.
		*/								
		String stringVal;
		
		/**
		* Boolean value of this object entry.
		*/								
		bool boolVal;		
		
		/**
		* Length of this object entry if its type is ARRAY_ENTRY.
		*/										
		int length;
		
		/**
		* Adds an empty child entry.
		* @param name Lookup key for the object entry.
		* @return The added object entry.
		*/				
		ObjectEntry *addChild(const String& name) {
			ObjectEntry *entry = new ObjectEntry();
			entry->type = ObjectEntry::CONTAINER_ENTRY;			
			entry->name = name;
			children.push_back(entry);
			length = children.size();
			return entry;
		}
		
		/**
		* Adds an empty child entry with a number value.
		* @param name Lookup key for the object entry.
		* @param val Number to set as value in the object entry.
		* @return The added object entry.
		*/						
		ObjectEntry *addChild(const String& name, Number val) {
			ObjectEntry *entry = new ObjectEntry();
			entry->type = ObjectEntry::FLOAT_ENTRY;			
			entry->NumberVal = val;
			entry->name = name;
			children.push_back(entry);
			length = children.size();			
			return entry;			
		}

		/**
		* Adds an empty child entry with an integer value.
		* @param name Lookup key for the object entry.
		* @param val Integer to set as value in the object entry.
		* @return The added object entry.
		*/						

		ObjectEntry *addChild(const String& name, int val) {
			ObjectEntry *entry = new ObjectEntry();
			entry->type = ObjectEntry::INT_ENTRY;
			entry->intVal = val;
			entry->name = name;
			children.push_back(entry);	
			length = children.size();			
			return entry;			
		}
		
		/**
		* Adds an empty child entry with a string value.
		* @param name Lookup key for the object entry.
		* @param val String to set as value in the object entry.
		* @return The added object entry.
		*/						
		ObjectEntry *addChild(const String& name, const String& val) {
			ObjectEntry *entry = new ObjectEntry();
			entry->type = ObjectEntry::STRING_ENTRY;
			entry->stringVal = val;
			entry->name = name;
			children.push_back(entry);
			length = children.size();			
			return entry;			
		}		
		

		/**
		* Adds an empty child entry with a boolean value.
		* @param name Lookup key for the object entry.
		* @param val Boolean to set as value in the object entry.
		* @return The added object entry.
		*/								
		ObjectEntry *addChild(const String& name, bool val) {
			ObjectEntry *entry = new ObjectEntry();
			entry->type = ObjectEntry::BOOL_ENTRY;			
			entry->boolVal = val;
			entry->name = name;
			children.push_back(entry);
			length = children.size();			
			return entry;			
		}
		
		ObjectEntry *addChild(ObjectEntry *entry) {
			children.push_back(entry);
			length = children.size();			
			return entry;			
		}		
		
		void Clear();
		
		static const int FLOAT_ENTRY = 0;		
		static const int INT_ENTRY = 1;
		static const int BOOL_ENTRY = 2;
		static const int ARRAY_ENTRY = 3;
		static const int STRING_ENTRY = 4;	
		static const int CONTAINER_ENTRY = 5;	

		/**
		* Accesses an object entry as an array by an integer lookup.
		* @param index Lookup index to return value for.
		* @return Object entry corresponding to the lookup index or NULL if one doesn't exist.
		*/		
		inline ObjectEntry *operator [] ( int index) { return children[index];}
		
		/**
		* Accesses an object entry by a string lookup and returns the corresponding object entry.
		* @param key Lookup key to return value for.
		* @return Object entry corresponding to the string value or NULL if one doesn't exist.
		*/
		inline ObjectEntry *operator [] ( const String& key) { for(int i=0; i < children.size(); i++) { if(children[i]->name == key) { return children[i]; } } return NULL; }
		
		std::vector<ObjectEntry*> children;
	};
	
	/**
	* Basic dictionary data object. Objects can store organized data and save and load it from disk. An object contains a hierarchy of ObjectEntry classes which hold the actual data.
	*/
	
	class _PolyExport Object {
	public:
		/**
		* Default constructor
		*/	
		Object();
		virtual ~Object();
		
		/**
		* Loads data from XML file into the object. 
		* @param fileName Path to the XML file to load.
		* @return Returns true is succesful, false if otherwise.
		*/		
		bool loadFromXML(const String& fileName);

		/**
		* Loads data from XML string into the object. 
		* @param xmlString XML data in a string.
		* @return Returns true is succesful, false if otherwise.
		*/				
		bool loadFromXMLString(const String &xmlString);
		
		/**
		* Saves the object to an XML file.
		* @param fileName Path to the XML file to save to.
		*/				
		void saveToXML(const String& fileName);
		
		void createFromXMLElement(TiXmlElement *element, ObjectEntry *entry);
		TiXmlElement *createElementFromObjectEntry(ObjectEntry *entry);
		
		/**
		* Root object entry.
		*/
		ObjectEntry root;
		
	};
	
}
