//
//  Archive.hpp
//  RGAssignment4
//
//  Created by rick gessner on 1/24/21.
//

#ifndef Archive_hpp
#define Archive_hpp

#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include<functional>
#include <vector>
#include <cstring>

namespace ECE141 {

    enum class ActionType {added, extracted, removed, listed, dumped};
    enum class AccessMode {AsNew, AsExisting}; //you can change values (but not names) of this enum

    struct ArchiveObserver {
        void operator()(ActionType anAction,
                        const std::string &aName, bool status);

    protected:
        std::vector<ActionType> ActionTypeList;
        std::string observerName;
        bool observerStatus;
    };

    //--------------------------------------------------------------------------------
    
    //You'll need to define your own classes for Blocks, and other useful types...
    //--------------------------------------------------------------------------------

    struct Block {
        //std::string name = "arxiv0";
        char title[50];
        int index; // index of the blocks
        int size; // size of this block
        bool available;
        char payload[1024 - sizeof(available) - sizeof(title) - sizeof(index) - sizeof(size)];
    };

    //Block2 

    class Archive {
    protected:
        Archive(const std::string &aFullPath, AccessMode aMode);  //protected on purpose
        std::string theArchiveName;
        AccessMode thisMode;
        ArchiveObserver* theObserver;
        std::fstream archiveStream;

    public:

        ~Archive() {
            archiveStream.close();
            //delete this;
        }//
        Archive(Archive& theArchive) {
            theArchiveName = theArchive.theArchiveName;
            thisMode = theArchive.thisMode;
            //archiveStream = theArchive.archiveStream;
        }
        static    Archive* createArchive(const std::string& anArchiveName) {
            //return nullptr;
            // create an Archive contains the blocks 
            std::string newArchiveName = anArchiveName;
            if (anArchiveName.find(".arc") == std::string::npos) {
                newArchiveName = anArchiveName + ".arc";
            }
            //archiveStream.open();

            Archive* thisArchive = new Archive(newArchiveName, AccessMode::AsNew);
            thisArchive->archiveStream.open(newArchiveName, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

            return thisArchive;
            
        }

        static    Archive* openArchive(const std::string& anArchiveName) {
            //std::string theArchiveName = anArchiveName + ".arc";
            std::string newArchiveName = anArchiveName;
            if (anArchiveName.find(".arc") == std::string::npos) {
                newArchiveName = anArchiveName + ".arc";
            }
            if (!std::filesystem::exists(newArchiveName.c_str())) {
                return nullptr;
            }
            Archive* thisArchive = new Archive(newArchiveName, AccessMode::AsExisting);
            thisArchive->archiveStream.open(newArchiveName, std::ios::in | std::ios::out | std::ios::binary);
            return thisArchive;
        }


        Archive&  addObserver(ArchiveObserver &anObserver);

        bool      add(const std::string& aFilename) {
            std::fstream theFile(aFilename, std::ios::in | std::ios::binary);
            const int payloadSize = 950;
            char* buffer = new char[payloadSize];
            int cur = theFile.tellg();
            theFile.seekg(0, std::ios::end);
            int fileSize = theFile.tellg();
            std::string theName;
            int i = 0;
            //while ()
            while (cur < fileSize) {
                theFile.seekg(cur);
                //int end = theFine.endFile
                theFile.read(buffer, std::min(payloadSize, fileSize - cur));
                Block newBlock;
                newBlock.index = i;
                newBlock.size = std::min(payloadSize, fileSize - cur);
                newBlock.available = false;
                memcpy(newBlock.payload, buffer, newBlock.size);
                theName = aFilename.substr(aFilename.find_last_of("/") + 1, aFilename.length()); // try to delete the folder
                memcpy(newBlock.title, theName.c_str(), sizeof(aFilename));
                cur += payloadSize;
                i += 1;
                archiveStream.write(reinterpret_cast<const char*>(&newBlock), sizeof(newBlock));
            }
            delete buffer;
            //debugDump(std::cout);
            return true;
        }
        bool      extract(const std::string& aFilename, const std::string& aFullPath);
        bool      remove(const std::string &aFilename);

        size_t    list(std::ostream& aStream);
        size_t    debugDump(std::ostream &aStream);
        // 1. start - end
        // 2. block that is avaible
        // 3. copy the data from the next to this one until the end of block
        // delete the old one
        size_t    compact();
        std::string getFullPath() const; //get archive path (including .arc extension)

        //STUDENT: add anything else you want here, (e.g. blocks?)...

    };

}

#endif /* Archive_hpp */
