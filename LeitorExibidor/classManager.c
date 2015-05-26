#define CLASSMANAGER_SERV
#include <stdio.h>
#include "lib/classManager.h"
#include "lib/exceptionManager.h"

ClassFile * classRead(FILE * dot_class){
    ClassFile * class_file;

    class_file = (ClassFile *)malloc(sizeof(ClassFile)); //Aloca espaço na memória para o arquivo .class

    class_file->magic = u4Read(dot_class);
    if(class_file->magic != U4_MAGIC) //Verifica se o arquivo informado eh um bytecode java
        throwException(NOT_BYTECODE_JAVA, NOT_BYTECODE_JAVA_MSG);

    class_file->minor_version = u2Read(dot_class);
    class_file->major_version = u2Read(dot_class);
    class_file->constant_pool_count = u2Read(dot_class);
    class_file->constant_pool = (cp_info *)malloc(class_file->constant_pool_count * sizeof(cp_info)); //aloca espaço para o constant_pool

    printf("Magic Number: 0x%08x\n",class_file->magic);
    printf("Minor Version: %d\n",class_file->minor_version);
    printf("Major Version: %d\n",class_file->major_version);
    printf("Constant Pool Counter: %d\n",class_file->constant_pool_count);


    for(int i = 1; i < class_file->constant_pool_count; i++){ //Lembre-se, o constant_pool começa em 1 !!!
        class_file->constant_pool->tag = u1Read(dot_class);
        printf("\nConstant Pool Tag: %d\n",class_file->constant_pool->tag);

        switch(class_file->constant_pool->tag){ //Dependendo do valor da TAG, será utilizado uma diferente struct da Union do Constant Pool
            case CONSTANT_Class:
                class_file->constant_pool->Class.name_index = u2Read(dot_class);
                    printf("\tNAME_INDEX: %d\n",class_file->constant_pool->Class.name_index);
                break;
            case CONSTANT_Fieldref:
                class_file->constant_pool->Fieldref.class_index = u2Read(dot_class);
                class_file->constant_pool->Fieldref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool->Fieldref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool->Fieldref.name_and_type_index);
                break;
            case CONSTANT_Methodref:
                class_file->constant_pool->Methodref.class_index = u2Read(dot_class);
                class_file->constant_pool->Methodref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool->Methodref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool->Methodref.name_and_type_index);
                break;
            case CONSTANT_InterfaceMethodref:
                class_file->constant_pool->InterfaceMethodref.class_index = u2Read(dot_class);
                class_file->constant_pool->InterfaceMethodref.name_and_type_index = u2Read(dot_class);
                    printf("\tCLASS_INDEX: %d\n",class_file->constant_pool->InterfaceMethodref.class_index);
                    printf("\tNAME_AND_TYPE_INDEX: %d\n",class_file->constant_pool->InterfaceMethodref.name_and_type_index);
                break;
            case CONSTANT_String:
                class_file->constant_pool->String.string_index = u2Read(dot_class);
                    printf("\tSTRING_INDEX: %d\n",class_file->constant_pool->String.string_index);
                break;
            case CONSTANT_Integer:
                class_file->constant_pool->Integer.bytes = u4Read(dot_class);
                    printf("\tBYTES: %d\n",class_file->constant_pool->Integer.bytes);
                break;
            case CONSTANT_Float:
                class_file->constant_pool->Float.bytes = u4Read(dot_class);
                    printf("\tBYTES: %d\n",class_file->constant_pool->Float.bytes);
                break;
            case CONSTANT_Long:
                class_file->constant_pool->Long.high_bytes = u4Read(dot_class);
                class_file->constant_pool->Long.low_bytes = u4Read(dot_class);
                    printf("\tHIGH_BYTES: %d\n",class_file->constant_pool->Long.high_bytes);
                    printf("\tLOW_BYTES: %d\n",class_file->constant_pool->Long.low_bytes);
                break;
            case CONSTANT_Double:
                class_file->constant_pool->Double.high_bytes = u4Read(dot_class);
                class_file->constant_pool->Double.low_bytes = u4Read(dot_class);
                    printf("\tHIGH_BYTES: %d\n",class_file->constant_pool->Double.high_bytes);
                    printf("\tLOW_BYTES: %d\n",class_file->constant_pool->Double.low_bytes);
                break;
            case CONSTANT_NameAndType:
                class_file->constant_pool->NameAndType.name_index = u2Read(dot_class);
                class_file->constant_pool->NameAndType.descriptor_index = u2Read(dot_class);
                    printf("\tNAME_INDEX: %d\n",class_file->constant_pool->NameAndType.name_index);
                    printf("\tDESCRIPTOR_INDEX: %d\n",class_file->constant_pool->NameAndType.descriptor_index);
                break;
            case CONSTANT_Utf8:
                    class_file->constant_pool->UTF8.length = u2Read(dot_class);
                        printf("\tLENGTH: %d\n",class_file->constant_pool->UTF8.length);
                        class_file->constant_pool->UTF8.bytes = (u1 *)malloc((class_file->constant_pool->UTF8.length * sizeof(u1)) + 1);
                    for(int j = 0; j < class_file->constant_pool->UTF8.length; j++){
                        class_file->constant_pool->UTF8.bytes[j] = u1Read(dot_class);
                            printf("\t\tBYTES: 0x%04x\n",class_file->constant_pool->UTF8.bytes[j]);
                    }
                    class_file->constant_pool->UTF8.bytes[class_file->constant_pool->UTF8.length] = '\0';
                    printf("\t\tBYTES CHAR: %s\n",class_file->constant_pool->UTF8.bytes);
                break;
            default:
                break;
        }
        //system("pause");
    }

    class_file->access_flags = u2Read(dot_class);
    class_file->this_class = u2Read(dot_class);
    class_file->super_class = u2Read(dot_class); //Se for ZERO a classe estende a classe OBJECT
    class_file->interfaces_count = u2Read(dot_class);
    class_file->interfaces = (u2 *)malloc(class_file->interfaces_count * sizeof(u2));

    printf("ACCESS_FLAGS: 0x%04x\n",class_file->access_flags);
    printf("THIS_CLASS: %d\n",class_file->this_class);
    printf("SUPER_CLASS: %d\n",class_file->super_class);
    printf("Interfaces Counter: %d\n",class_file->interfaces_count);

    for(int i = 0; i < class_file->interfaces_count; i++){
        class_file->interfaces = u2Read(dot_class);
            printf("\tINTERFACES: %d\n",class_file->interfaces);
    }

    class_file->fields_count = u2Read(dot_class);
    printf("Fields Counter: %d\n",class_file->fields_count);

    return class_file;
}
