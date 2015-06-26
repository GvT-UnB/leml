#define CLASSMANAGER_SERV
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "lib/UserViewer.h"

///Funcoes de Debug (Serao apagadas depois de cumprirem sua funcao)
void printFrame(Frame *frame);
void printHandler(ClassHandler *handler);
void printFields(u2 fields_count, field_info *fields);
void printLocalVariableArray(u2 attributes_count, LocalVariable *localVariableArray);
void printOperandStack(structOperandStack ** operandStack);


void classPrint(ClassFile * class_file){
    char string[100];

	printf("Magic Number:\t\t0x%08x\n",class_file->magic);
    printf("Minor Version:\t\t%d\n",class_file->minor_version);
    if(class_file->major_version > 48) //Condição para imprimir o número da versão comercial do Java
        printf("Major Version:\t\t%d [%d]\n",class_file->major_version, class_file->major_version%44);
    else
        printf("Major Version:\t\t%d [1.%d]\n",class_file->major_version, class_file->major_version%44);

    printf("Constant Pool Count:\t%d\n",class_file->constant_pool_count);
    string[0] = '\0';
    getAccessFlag(class_file->access_flags, &string, 0);
    printf("Access flags:\t\t0x%04x [%s]\n",class_file->access_flags, string);
    string[0] = '\0';
    selectPointer(class_file, class_file->this_class, &string, 0);
    printf("This class:\t\tcp_info #%d <%s>\n",class_file->this_class, string);
    string[0] = '\0';
    selectPointer(class_file, class_file->super_class, &string, 0);
    printf("Super class:\t\tcp_info #%d <%s>\n",class_file->super_class, string);
    printf("Interfaces Count:\t%d\n",class_file->interfaces_count);
    printf("Fields Count:\t\t%d\n",class_file->fields_count);
    printf("Methods Count:\t\t%d\n",class_file->methods_count);
    printf("Attributes Count:\t%d\n",class_file->attributes_count);

	printf("\n+Constant Pool:\n");
	printConstantPool(class_file->constant_pool,class_file->constant_pool_count, class_file);

    printf("\n+Interfaces\n");
	for(int i = 0; i < class_file->interfaces_count; i++){
        string[0] = '\0';
        selectPointer(class_file, class_file->interfaces[i], &string, 0);
		printf("\tInterface %d:\t\tcp_info #%d <%s>\n", i,class_file->interfaces[i], string);
	}
	system("pause");
	printf("\n+Fields\n");
    printf("\tMember Count:\t\t%d\n",class_file->fields_count);
	for(int i = 0; i < class_file->fields_count; i++){
        string[0] = '\0';
        selectPointer(class_file, class_file->fields[i].name_index, &string, 0);
        printf("\n\t[%d] %s", i, string);
		printf("\tName:\t\t\t\tcp_info #%d <%s>\n",class_file->fields[i].name_index, string);
		string[0] = '\0';
        selectPointer(class_file, class_file->fields[i].descriptor_index, &string, 0);
        printf("\t\t\tDescriptor:\t\t\tcp_info #%d <%s>\n",class_file->fields[i].descriptor_index, string);
        string[0] = '\0';
        getAccessFlag(class_file->fields[i].access_flags, &string, 0);
        printf("\t\t\tAccess flags:\t\t\t0x%04x [%s]\n",class_file->fields[i].access_flags, string);

        printAttributesInfo(class_file->fields[i].attributes, class_file->fields[i].attributes_count, class_file->constant_pool, class_file);
 	}



    system("pause");
    printf("\n+Methods\n");
    printf("\tMember Count:\t\t%d\n",class_file->methods_count);
	///Chama a função responsavl por printar os methods
	printMethodInfo(class_file->methods, class_file->methods_count, class_file);

    printf("\n+Attributes\n");
	printAttributesInfo(class_file->attributes,class_file->attributes_count, class_file->constant_pool, class_file);
}

void printConstantPool(cp_info * constant_pool, u2 constant_pool_count, ClassFile *class_file){
    char string[100];
    u4 var_u4;
    float var_float;
    int64_t var_64;
    double var_double;
    for(int i = 1; i < constant_pool_count; i++){
		 switch(constant_pool[i].tag){
			case CONSTANT_Class:
			    string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				printf("\n[%02d]CONSTANT_Class:", i);
                printf("\t\tClass Name:\tcp_info #%d <%s>\n",constant_pool[i].Class.name_index, string);
				break;
			case CONSTANT_Fieldref:
			    string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				printf("\n[%02d]CONSTANT_Fieldref:", i);
				printf("\t\tClass name:\tcp_info #%d <%s>\n",constant_pool[i].Fieldref.class_index, string);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 1);
				printf("\t\t\t\tName and type:\tcp_info #%d <%s>\n",constant_pool[i].Fieldref.name_and_type_index, string);
				break;
			case CONSTANT_Methodref:
			    string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				printf("\n[%02d]CONSTANT_Methodref:", i);
				printf("\t\tClass name:\tcp_info #%d <%s>\n",constant_pool[i].Methodref.class_index, string);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 1);
				printf("\t\t\t\tName and type:\tcp_info #%d <%s>\n",constant_pool[i].Methodref.name_and_type_index, string);
				break;
			case CONSTANT_InterfaceMethodref:
				printf("\n[%02d]CONSTANT_InterfaceMethodref:",i);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				printf("\t\tClass name:\tcp_info #%d <%s>\n",constant_pool[i].InterfaceMethodref.class_index, string);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 1);
				printf("\t\t\t\tName and type:\tcp_info #%d <%s>\n",constant_pool[i].InterfaceMethodref.name_and_type_index, string);
				break;
			case CONSTANT_String:
				printf("\n[%02d]CONSTANT_String",i);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
                printf("\t\tString:\t\tcp_info #%d <%s>\n",constant_pool[i].String.string_index, string);
				break;
			case CONSTANT_Integer:
				printf("\n[%02d]CONSTANT_Integer", i);
                printf("\t\tBytes:\t\t\t0x%04x\n",constant_pool[i].Integer.bytes);
                printf("\t\t\t\tInteger:\t\t%d\n",constant_pool[i].Integer.bytes);
				break;
			case CONSTANT_Float:
				printf("\n[%02d]CONSTANT_Float", i);
				printf("\t\tBytes:\t0x%04x\n",constant_pool[i].Float.bytes);
				var_u4 = constant_pool[i].Float.bytes;
				var_float = *((float*)&var_u4);
				printf("\t\t\t\tFloat:\t%.4f\n",var_float);
				break;
			case CONSTANT_Long:
				printf("\n[%02d]CONSTANT_Long",i);
				printf("\t\tHIGH_BYTES:\t\t0x%08x\n",constant_pool[i].Long.high_bytes);
				printf("\t\t\t\tLOW_BYTES:\t\t0x%04x\n",constant_pool[i].Long.low_bytes);
				var_64 = constant_pool[i].Long.high_bytes;
				var_64 = (var_64 << 32) | constant_pool[i].Long.low_bytes;
				printf("\t\t\t\tLong:\t\t\t%ld\n", var_64);
                i++;
                printf("\n[%02d](large numeric continued)\n", i);
				break;
			case CONSTANT_Double:
				printf("\n[%02d]CONSTANT_Double",i);
				printf("\t\tHigh bytes:\t\t0x%08x\n",constant_pool[i].Double.high_bytes);
				printf("\t\t\t\tLow bytes:\t\t0x%08x\n",constant_pool[i].Double.low_bytes);
                var_64 = constant_pool[i].Double.high_bytes;
                var_64 = (var_64 << 32) | constant_pool[i].Double.low_bytes;
                var_double = *((double*)&var_64);
                printf("\t\t\t\tDouble:\t\t\t%f\n", var_double);
				i++;
				printf("\n[%02d](large numeric continued)\n", i);
				break;
			case CONSTANT_NameAndType:
				printf("\n[%02d]CONSTANT_NameAndType", i);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 0);
				printf("\tName:\t\tcp_info #%d <%s>\n",constant_pool[i].NameAndType.name_index, string);
				string[0] = '\0';
			    selectPointer(class_file, i, &string, 1);
				printf("\t\t\t\tDescriptor:\tcp_info #%d <%s>\n",constant_pool[i].NameAndType.descriptor_index, string);
				break;
			case CONSTANT_Utf8:
				printf("\n[%02d]CONSTANT_Utf8",i);
				printf("\t\tLength of byte array:\t%d\n",constant_pool[i].UTF8.length);
				printf("\t\t\t\tLength of string:\t%d\n",constant_pool[i].UTF8.length);
				printf("\t\t\t\tString:\t\t\t%s\n",constant_pool[i].UTF8.bytes);
				break;
			default:
                break;
		 }
        if(i%5==0)
            system("pause");
	}
}

void printMethodInfo(method_info * methods, u2 methods_count, ClassFile *class_file){
    char string[100];
    for(int i = 0; i < methods_count; i++){
        string[0] = '\0';
        selectPointer(class_file, methods[i].name_index, &string, 0);
        printf("\n\t[%d] %s\n", i, string);
		printf("\t\tName:\t\t\tcp_info #%d <%s>\n",methods[i].name_index, string);
		string[0] = '\0';
        selectPointer(class_file, methods[i].descriptor_index, &string, 0);
		printf("\t\tDescriptor:\t\tcp_info #%d <%s>\n",methods[i].descriptor_index, string);
		string[0] = '\0';
        getAccessFlag(methods[i].access_flags, &string, 1);
		printf("\t\tAccess flags:\t\t0x%04x [%s]\n",methods[i].access_flags, string);

		printAttributesInfo(methods[i].attributes,methods[i].attributes_count, class_file->constant_pool, class_file);
	}
}

void printAttributesInfo(attribute_info * attributes, u2 attributes_count, cp_info * constant_pool, ClassFile *class_file){
	char *indice;
	char string[100];

	for(int j = 0; j < attributes_count; j++){
		indice = constant_pool[attributes[j].name_index].UTF8.bytes;

		printf("\n\t\t[%d] %s\n", j, indice);
        printf("\t\t\tAttribute name index:\t\tcp_info #%d\n",attributes[j].name_index);
        printf("\t\t\tAttribute length:\t\t%d\n",attributes[j].length);

		switch(attributes[j].tag){
			case ATTRIBUTE_ConstantValue:
                string[0] = '\0';
                selectPointer(class_file, attributes[j].ConstantValue.index, &string, 0);
				printf("\t\t\tConstant value index:\t\tcp_info #%d <%s>\n",attributes[j].ConstantValue.index, string);
				break;
			case ATTRIBUTE_Code:
			    printf("\t\tBytecode:\n");
			    printf("\t\t-------------------------------------------\n");
				printCode(attributes[j].Code.code, attributes[j].Code.code_length, constant_pool);
				printf("\t\t-------------------------------------------\n");
				printf("\n\t\tException table:\n");
				printf("\t----------------------------------------------------------------------\n");
				printf("\tNr.  |  start_pc  |  end_pc  |  handler_pc  |  catch_type  |  verbose\n");
				for(int k =0; k < attributes[j].Code.exception_table_length; k++){
                    printf("\t\t%d\t|\t%d\t|\t%d\t|\t%d\t|\t%d\n", k, attributes[j].Code.ExceptionTable[k].start_pc, \
                           attributes[j].Code.ExceptionTable[k].end_pc, attributes[j].Code.ExceptionTable[k].handler_pc, \
                           attributes[j].Code.ExceptionTable[k].catch_type);
                }
                printf("\t----------------------------------------------------------------------\n");
				printf("\n\t\tMisc:\n");
				printf("\t\tMaximum stack depth:\t\t%d\n",attributes[j].Code.max_stack);
				printf("\t\tMaximum local variables:\t%d\n",attributes[j].Code.max_locals);
				printf("\t\tCode length:\t\t\t%d\n",attributes[j].Code.code_length);

				printAttributesInfo(attributes[j].Code.attributes, attributes[j].Code.attributes_count,class_file->constant_pool, class_file);

				break;
			case ATTRIBUTE_Exceptions:
				printf("\t\tExceptions.number_of_exceptions: %d\n",attributes[j].Exceptions.number_of_exceptions);
				for(int k =0; k < attributes[j].Exceptions.number_of_exceptions; k++){
					printf("\t\tExceptions.exception_index_table[%d]: %d\n",k,attributes[j].Exceptions.exception_index_table[k]);
				}
				break;
			case ATTRIBUTE_InnerClasses:
				printf("\t\tInnerClasses.number_of_classes: %d\n",attributes[j].InnerClasses.number_of_classes);
				break;
			case ATTRIBUTE_Syntethic:
				break;
			case ATTRIBUTE_LineNumber:
				printf("\t\t\tNr.\t    start_pc\t    line_number\n");
				printf("\t\t-------------------------------------------------\n");
				for(int i=0; i<attributes[j].LineNumber.line_number_table_length; i++){
                    printf("\t\t\t%d\t\t%d\t\t%d\n", i, attributes[j].LineNumber.Line_number_table[i].start_pc, attributes[j].LineNumber.Line_number_table[i].line_number);
				}
				printf("\t\t-------------------------------------------------\n");
                break;
			case ATTRIBUTE_LocalVariable:
				printf("\t\tLocalVariable.local_variable_table_length: %d\n",attributes[j].LocalVariable.local_variable_table_length);
				break;
			case ATTRIBUTE_SourceFile:
			    string[0] = '\0';
                selectPointer(class_file, attributes[j].SourceFile.sourcefile_index, &string, 0);
				printf("\t\t\tSource file name index:\t\tcp_info #%d <%s>\n",attributes[j].SourceFile.sourcefile_index, string);
                break;
			default: //ATTRIBUTE_Unknown
                printf("\t\tATRIBUTO DESCONHECIDO:\n");
                break;
		}

	}

}


void getAccessFlag(int flag, char *string, int tipo){
    switch (tipo){
    case (0):
        if(flag == 0){
            return;
        }else if(flag%ACC_STRICT == 0){
            flag -= ACC_STRICT;
            getAccessFlag(flag, string, tipo);
            strcat(string, "strict ");
        }else if(flag%ACC_ABSTRACT == 0){
            flag -= ACC_ABSTRACT;
            getAccessFlag(flag, string, tipo);
            strcat(string, "abstract ");
        }else if(flag%ACC_INTERFACE == 0){
            printf("inter\n");
            flag -= ACC_INTERFACE;
            getAccessFlag(flag, string, tipo);
            strcat(string, "interface ");
        }else if(flag%ACC_NATIVE == 0){
            flag -= ACC_NATIVE;
            getAccessFlag(flag, string, tipo);
            strcat(string, "native ");
        }else if(flag%ACC_TRANSIENT == 0){
            flag -= ACC_TRANSIENT;
            getAccessFlag(flag, string, tipo);
            strcat(string, "transient ");
        }else if(flag%ACC_VOLATILE == 0){
            flag -= ACC_VOLATILE;
            getAccessFlag(flag, string, tipo);
            strcat(string, "volatile ");
        }else if(flag%33 == 0){ //considera o 0x0021 como public
            flag -= 33;
            getAccessFlag(flag, string, tipo);
            strcat(string, "public ");
        }else if(flag%ACC_SUPER == 0){
            flag -= ACC_SUPER;
            getAccessFlag(flag, string, tipo);
            strcat(string, "super ");
        }else if(flag%ACC_FINAL == 0){
            flag -= ACC_FINAL;
            getAccessFlag(flag, string, tipo);
            strcat(string, "final ");
        }else if(flag%ACC_STATIC == 0){
            flag -= ACC_STATIC;
            getAccessFlag(flag, string, tipo);
            strcat(string, "static ");
        }else if(flag%ACC_PROTECTED == 0){
            flag -= ACC_PROTECTED;
            getAccessFlag(flag, string, tipo);
            strcat(string, "protect ");
        }else if(flag%ACC_PRIVATE == 0){
            flag -= ACC_PRIVATE;
            getAccessFlag(flag, string, tipo);
            strcat(string, "private ");
        }else if(flag%ACC_PUBLIC == 0){
            flag -= ACC_PUBLIC;
            getAccessFlag(flag, string, tipo);
            strcat(string, "public ");
        }
        break;
    case (1):
        if(flag == 0){
            return;
        }else if(flag%ACC_STRICT == 0){
            flag -= ACC_STRICT;
            getAccessFlag(flag, string, tipo);
            strcat(string, "strict ");
        }else if(flag%ACC_ABSTRACT == 0){
            flag -= ACC_ABSTRACT;
            getAccessFlag(flag, string, tipo);
            strcat(string, "abstract ");
        }else if(flag%ACC_INTERFACE == 0){
            printf("inter\n");
            flag -= ACC_INTERFACE;
            getAccessFlag(flag, string, tipo);
            strcat(string, "interface ");
        }else if(flag%ACC_NATIVE == 0){
            flag -= ACC_NATIVE;
            getAccessFlag(flag, string, tipo);
            strcat(string, "native ");
        }else if(flag%ACC_TRANSIENT == 0){
            flag -= ACC_TRANSIENT;
            getAccessFlag(flag, string, tipo);
            strcat(string, "transient ");
        }else if(flag%ACC_VOLATILE == 0){
            flag -= ACC_VOLATILE;
            getAccessFlag(flag, string, tipo);
            strcat(string, "volatile ");
        }else if(flag%33 == 0){ //considera o 0x0021 como public
            flag -= 33;
            getAccessFlag(flag, string, tipo);
            strcat(string, "public ");
        }else if(flag%ACC_SUPER == 0){
            flag -= ACC_SYNCHRONIZED;
            getAccessFlag(flag, string, tipo);
            strcat(string, "syncronized ");
        }else if(flag%ACC_FINAL == 0){
            flag -= ACC_FINAL;
            getAccessFlag(flag, string, tipo);
            strcat(string, "final ");
        }else if(flag%ACC_STATIC == 0){
            flag -= ACC_STATIC;
            getAccessFlag(flag, string, tipo);
            strcat(string, "static ");
        }else if(flag%ACC_PROTECTED == 0){
            flag -= ACC_PROTECTED;
            getAccessFlag(flag, string, tipo);
            strcat(string, "protect ");
        }else if(flag%ACC_PRIVATE == 0){
            flag -= ACC_PRIVATE;
            getAccessFlag(flag, string, tipo);
            strcat(string, "private ");
        }else if(flag%ACC_PUBLIC == 0){
            flag -= ACC_PUBLIC;
            getAccessFlag(flag, string, tipo);
            strcat(string, "public ");
        }
        break;
    //case ('a'):
      //  break;
    }
}

//Função responsável por selecionar a opção dentro do CONSTANT_x quando esta possui mais de dois indices
void selectPointer(ClassFile * class_file, int indice, char *string, int option){

    switch (class_file->constant_pool[indice].tag){
    case (CONSTANT_Class):
        catchString(class_file, class_file->constant_pool[indice].Class.name_index, string);
        break;
    case (CONSTANT_String):
        catchString(class_file, class_file->constant_pool[indice].String.string_index, string);
        break;
    case (CONSTANT_Methodref):
        if(option == 0)
            catchString(class_file, class_file->constant_pool[indice].Methodref.class_index, string);
        else
            catchString(class_file, class_file->constant_pool[indice].Methodref.name_and_type_index, string);
        break;
    case (CONSTANT_Fieldref):
        if(option == 0)
            catchString(class_file, class_file->constant_pool[indice].Fieldref.class_index, string);
        else
            catchString(class_file, class_file->constant_pool[indice].Fieldref.name_and_type_index, string);
        break;
    case(CONSTANT_NameAndType):
        if(option == 0)
            catchString(class_file, class_file->constant_pool[indice].NameAndType.name_index, string);
        else
            catchString(class_file, class_file->constant_pool[indice].NameAndType.descriptor_index, string);
        break;
    case (CONSTANT_InterfaceMethodref):
        if(option == 0)
            catchString(class_file, class_file->constant_pool[indice].InterfaceMethodref.class_index, string);
        else
            catchString(class_file, class_file->constant_pool[indice].InterfaceMethodref.class_index, string);
        break;
    default:
        catchString(class_file, indice, string);
        break;
    }
}

//Função para montar a string caso ela possua referencia para um ponteiro que não seja um Utf8 \
Lembrando que uma vez que tenham dois ponteiros dentro de uma CONSTANT_x, as strings devem ser concatenadas
void catchString(ClassFile * class_file, int indice, char *string){

    u1 *s1;
    s1 = (u1*)malloc(100*sizeof(u1));
    int64_t var_64;
    float var_float;
    double var_double;

    switch (class_file->constant_pool[indice].tag){
    case (CONSTANT_Class):
        catchString(class_file, class_file->constant_pool[indice].Class.name_index, string);
        break;
    case (CONSTANT_String):
        catchString(class_file, class_file->constant_pool[indice].String.string_index, string);
        break;
    case (CONSTANT_Methodref):
        catchString(class_file, class_file->constant_pool[indice].Methodref.class_index, string);
        catchString(class_file, class_file->constant_pool[indice].Methodref.name_and_type_index, string);
        break;
    case (CONSTANT_Fieldref):
        catchString(class_file, class_file->constant_pool[indice].Fieldref.class_index, string);
        catchString(class_file, class_file->constant_pool[indice].Fieldref.name_and_type_index, string);
        break;
    case(CONSTANT_NameAndType):
        catchString(class_file, class_file->constant_pool[indice].NameAndType.name_index, string);
        catchString(class_file, class_file->constant_pool[indice].NameAndType.descriptor_index, string);
        break;
    case (CONSTANT_InterfaceMethodref):
        catchString(class_file, class_file->constant_pool[indice].InterfaceMethodref.class_index, string);
        catchString(class_file, class_file->constant_pool[indice].InterfaceMethodref.class_index, string);
        break;
    case (CONSTANT_Utf8):
        s1 = class_file->constant_pool[indice].UTF8.bytes;
        strcat(string, s1);
        break;
    case (CONSTANT_Integer):
        sprintf(s1, "%d", class_file->constant_pool[indice].Integer.bytes);
        strcat(string, s1);
        break;
    case (CONSTANT_Float):
        var_float = *((float*)&(class_file->constant_pool[indice].Float.bytes));
        sprintf(s1, "%f", var_float);
        strcat(string, s1);
        break;
    case (CONSTANT_Long):
        var_64 = class_file->constant_pool[indice].Long.high_bytes;
        var_64 = (var_64 << 32) | class_file->constant_pool[indice].Long.low_bytes;
        sprintf(s1, "%ld", var_64);
        strcat(string, s1);
        break;
    case (CONSTANT_Double):
        var_64 = class_file->constant_pool[indice].Double.high_bytes;
        var_64 = (var_64 << 32) | class_file->constant_pool[indice].Double.low_bytes;
        var_double = *((double*)&var_64);
        sprintf(s1, "%f", var_double);
        strcat(string, s1);
        break;
    }
}

void printCode(u1 *code, u4 code_length, cp_info * constant_pool){
    u2 var_u2;
    u2 index1, index2;
    for(int k = 0; k < code_length; k++){
    printf("\t\t\t%2d  ",k);
        switch(code[k]){
            case OPCODE_nop:
                printf("nop\n");
                break;
            case OPCODE_aconst_null:
                printf("aconst_null\n");
                break;
            case OPCODE_iconst_m1:
                printf("iconst_m1\n");
                break;
            case OPCODE_iconst_0:
                printf("iconst_0\n");
                break;
            case OPCODE_iconst_1:
                printf("iconst_1\n");
                break;
            case OPCODE_iconst_2:
                printf("iconst_2\n");
                break;
            case OPCODE_iconst_3:
                printf("iconst_3\n");
                break;
            case OPCODE_iconst_4:
                printf("iconst_4\n");
                break;
            case OPCODE_iconst_5:
                printf("iconst_5\n");
                break;
            case OPCODE_lconst_0:
                printf("lconst_0\n");
                break;
            case OPCODE_lconst_1:
                printf("lconst_1\n");
                break;
            case OPCODE_fconst_0:
                printf("fconst_0\n");
                break;
            case OPCODE_fconst_1:
                printf("fconst_1\n");
                break;
            case OPCODE_fconst_2:
                printf("fconst_2\n");
                break;
            case OPCODE_dconst_0:
                printf("dconst_0\n");
                break;
            case OPCODE_dconst_1:
                printf("dconst_1\n");
                break;
            case OPCODE_bipush:
                printf("bipush %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_sipush:
                printf("sipush %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ldc:
                printf("ldc %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_ldc_w:
                printf("ldc_w %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ldc2_w:
                printf("ldc2_w %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_iload:
                printf("iload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_lload:
                printf("lload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_fload:
                printf("fload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_dload:
                printf("dload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_aload:
                printf("aload %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_iload_0:
                printf("iload_0\n");
                break;
            case OPCODE_iload_1:
                printf("iload_1\n");
                break;
            case OPCODE_iload_2:
                printf("iload_2\n");
                break;
            case OPCODE_iload_3:
                printf("iload_3\n");
                break;
            case OPCODE_lload_0:
                printf("lload_0\n");
                break;
            case OPCODE_lload_1:
                printf("lload_1\n");
                break;
            case OPCODE_lload_2:
                printf("lload_2\n");
                break;
            case OPCODE_lload_3:
                printf("lload_3\n");
                break;
            case OPCODE_fload_0:
                printf("fload_0\n");
                break;
            case OPCODE_fload_1:
                printf("fload_1\n");
                break;
            case OPCODE_fload_2:
                printf("fload_2\n");
                break;
            case OPCODE_fload_3:
                printf("fload_3\n");
                break;
            case OPCODE_dload_0:
                printf("dload_0\n");
                break;
            case OPCODE_dload_1:
                printf("dload_1\n");
                break;
            case OPCODE_dload_2:
                printf("dload_2\n");
                break;
            case OPCODE_dload_3:
                printf("dload_3\n");
                break;
            case OPCODE_aload_0:
                printf("aload_0\n");
                break;
            case OPCODE_aload_1:
                printf("aload_1\n");
                break;
            case OPCODE_aload_2:
                printf("aload_2\n");
                break;
            case OPCODE_aload_3:
                printf("aload_3\n");
                break;
            case OPCODE_iaload:
                printf("iaload\n");
                break;
            case OPCODE_laload:
                printf("laload\n");
                break;
            case OPCODE_faload:
                printf("faload\n");
                break;
            case OPCODE_daload:
                printf("daload\n");
                break;
            case OPCODE_aaload:
                printf("aaload\n");
                break;
            case OPCODE_baload:
                printf("baload\n");
                break;
            case OPCODE_caload:
                printf("caload\n");
                break;
            case OPCODE_saload:
                printf("saload\n");
                break;
            case OPCODE_istore:
                printf("istore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_lstore:
                printf("lstore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_fstore:
                printf("fstore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_dstore:
                printf("dstore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_astore:
                printf("astore %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_istore_0:
                printf("istore_0\n");
                break;
            case OPCODE_istore_1:
                printf("istore_1\n");
                break;
            case OPCODE_istore_2:
                printf("istore_2\n");
                break;
            case OPCODE_istore_3:
                printf("istore_3\n");
                break;
            case OPCODE_lstore_0:
                printf("lstore_0\n");
                break;
            case OPCODE_lstore_1:
                printf("lstore_1\n");
                break;
            case OPCODE_lstore_2:
                printf("lstore_2\n");
                break;
            case OPCODE_lstore_3:
                printf("lstore_3\n");
                break;
            case OPCODE_fstore_0:
                printf("fstore_0\n");
                break;
            case OPCODE_fstore_1:
                printf("fstore_1\n");
                break;
            case OPCODE_fstore_2:
                printf("fstore_2\n");
                break;
            case OPCODE_fstore_3:
                printf("fstore_3\n");
                break;
            case OPCODE_dstore_0:
                printf("dstore_0\n");
                break;
            case OPCODE_dstore_1:
                printf("dstore_1\n");
                break;
            case OPCODE_dstore_2:
                printf("dstore_2\n");
                break;
            case OPCODE_dstore_3:
                printf("dstore_3\n");
                break;
            case OPCODE_astore_0:
                printf("astore_0\n");
                break;
            case OPCODE_astore_1:
                printf("astore_1\n");
                break;
            case OPCODE_astore_2:
                printf("astore_2\n");
                break;
            case OPCODE_astore_3:
                printf("astore_3\n");
                break;
            case OPCODE_iastore:
                printf("iastore\n");
                break;
            case OPCODE_lastore:
                printf("lastore\n");
                break;
            case OPCODE_fastore:
                printf("fastore\n");
                break;
            case OPCODE_dastore:
                printf("dastore\n");
                break;
            case OPCODE_aastore:
                printf("aastore\n");
                break;
            case OPCODE_bastore:
                printf("bastore\n");
                break;
            case OPCODE_castore:
                printf("castore:\n");
                break;
            case OPCODE_sastore:
                printf("sastore\n");
                break;
            case OPCODE_pop:
                printf("pop\n");
                break;
            case OPCODE_pop2:
                printf("pop2\n");
                break;
            case OPCODE_dup:
                printf("dup\n");
                break;
            case OPCODE_dup_x1:
                printf("dup_x1\n");
                break;
            case OPCODE_dup_x2:
                printf("dup_x2\n");
                break;
            case OPCODE_dup2:
                printf("dup2\n");
                break;
            case OPCODE_dup2_x1:
                printf("dup2_x1\n");
                break;
            case OPCODE_dup2_x2:
                printf("dup2_x2\n");
                break;
            case OPCODE_swap:
                printf("swap\n");
                break;
            case OPCODE_iadd:
                printf("iadd\n");
                break;
            case OPCODE_ladd:
                printf("ladd\n");
                break;
            case OPCODE_fadd:
                printf("fadd\n");
                break;
            case OPCODE_dadd:
                printf("dadd\n");
                break;
            case OPCODE_isub:
                printf("isub\n");
                break;
            case OPCODE_lsub:
                printf("lsub\n");
                break;
            case OPCODE_fsub:
                printf("fsub\n");
                break;
            case OPCODE_dsub:
                printf("dsub:\n");
                break;
            case OPCODE_imul:
                printf("imul\n");
                break;
            case OPCODE_lmul:
                printf("lmul\n");
                break;
            case OPCODE_fmul:
                printf("fmul\n");
                break;
            case OPCODE_dmul:
                printf("dmul\n");
                break;
            case OPCODE_idiv:
                printf("idiv\n");
                break;
            case OPCODE_ldiv:
                printf("ldiv\n");
                break;
            case OPCODE_fdiv:
                printf("fdiv\n");
                break;
            case OPCODE_ddiv:
                printf("ddiv\n");
                break;
            case OPCODE_irem:
                printf("irem\n");
                break;
            case OPCODE_lrem:
                printf("lrem\n");
                break;
            case OPCODE_frem:
                printf("frem\n");
                break;
            case OPCODE_drem:
                printf("drem\n");
                break;
            case OPCODE_ineg:
                printf("ineg\n");
                break;
            case OPCODE_lneg:
                printf("lneg\n");
                break;
            case OPCODE_fneg:
                printf("fneg\n");
                break;
            case OPCODE_dneg:
                printf("dneg\n");
                break;
            case OPCODE_ishl:
                printf("ishl\n");
                break;
            case OPCODE_lshl:
                printf("lshl\n");
                break;
            case OPCODE_ishr:
                printf("ishr\n");
                break;
            case OPCODE_lshr:
                printf("lshr\n");
                break;
            case OPCODE_iushr:
                printf("iushr\n");
                break;
            case OPCODE_lushr:
                printf("lushr\n");
                break;
            case OPCODE_iand:
                printf("iand\n");
                break;
            case OPCODE_land:
                printf("land\n");
                break;
            case OPCODE_ior:
                printf("ior\n");
                break;
            case OPCODE_lor:
                printf("lor\n");
                break;
            case OPCODE_ixor:
                printf("ixor\n");
                break;
            case OPCODE_lxor:
                printf("lxor\n");
                break;
            case OPCODE_iinc:
                printf("iinc %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_i2l:
                printf("i2l\n");
                break;
            case OPCODE_i2f:
                printf("i2f\n");
                break;
            case OPCODE_i2d:
                printf("i2d\n");
                break;
            case OPCODE_l2i:
                printf("l2i\n");
                break;
            case OPCODE_l2f:
                printf("l2f\n");
                break;
            case OPCODE_l2d:
                printf("l2d\n");
                break;
            case OPCODE_f2i:
                printf("f2i\n");
                break;
            case OPCODE_f2l:
                printf("f2l\n");
                break;
            case OPCODE_f2d:
                printf("f2d\n");
                break;
            case OPCODE_d2i:
                printf("d2i\n");
                break;
            case OPCODE_d2l:
                printf("d2l\n");
                break;
            case OPCODE_d2f:
                printf("d2f\n");
                break;
            case OPCODE_i2b:
                printf("i2b\n");
                break;
            case OPCODE_i2c:
                printf("i2c\n");
                break;
            case OPCODE_i2s:
                printf("i2s\n");
                break;
            case OPCODE_lcmp:
                printf("lcmp\n");
                break;
            case OPCODE_fcmpl:
                printf("fcmpl\n");
                break;
            case OPCODE_fcmpg:
                printf("fcmpg\n");
                break;
            case OPCODE_dcmpl:
                printf("dcmpl\n");
                break;
            case OPCODE_dcmpg:
                printf("dcmpg\n");
                break;
            case OPCODE_ifeq:
                printf("ifeq %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifne:
                printf("ifne %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_iflt:
                printf("iflt %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifge:
                printf("ifge %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifgt:
                printf("ifgt %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifle:
                printf("ifle %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmpeq:
                printf("if_icmpeq %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmpne:
                printf("if_icmpne %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmplt:
                printf("if_icmplt %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmpge:
                printf("if_icmpge %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmpgt:
                printf("if_icmpgt %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_icmple:
                printf("if_icmple %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_acmpeq:
                printf("if_acmpeq %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_if_acmpne:
                printf("if_acmpne\n");
                break;
            case OPCODE_goto:
                printf("goto %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_jsr:
                printf("jsr %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ret:
                printf("ret %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_tableswitch:
                printf("tableswitch\n");
                break;
            case OPCODE_lookupswitch:
                printf("lookupswitch\n");
                break;
            case OPCODE_ireturn:
                printf("ireturn\n");
                break;
            case OPCODE_lreturn:
                printf("lreturn\n");
                break;
            case OPCODE_freturn:
                printf("freturn\n");
                break;
            case OPCODE_dreturn:
                printf("dreturn\n");
                break;
            case OPCODE_areturn:
                printf("areturn\n");
                break;
            case OPCODE_return:
                printf("return\n");
                break;
            case OPCODE_getstatic:
                printf("getstatic %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_putstatic:
                printf("putstatic %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_getfield:
                printf("getfield %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_putfield:
                printf("putfield %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_invokevirtual:
                var_u2 = (code[k+1] << 8) | code[k+2];
                index1 = constant_pool[var_u2].Methodref.class_index;
                index1 = constant_pool[index1].Class.name_index;
                index2 = constant_pool[var_u2].Methodref.name_and_type_index;
                index2 = constant_pool[index2].NameAndType.name_index;
                printf("invokevirtual %d <%s.%s>\n", var_u2, constant_pool[index1].UTF8.bytes, constant_pool[index2].UTF8.bytes);
                k=k+2;
                break;
            case OPCODE_invokespecial:
                var_u2 = (code[k+1] << 8) | code[k+2];
                index1 = constant_pool[var_u2].Methodref.class_index;
                index1 = constant_pool[index1].Class.name_index;
                index2 = constant_pool[var_u2].Methodref.name_and_type_index;
                index2 = constant_pool[index2].NameAndType.name_index;
                printf("invokespecial %d <%s.%s>\n", var_u2, constant_pool[index1].UTF8.bytes, constant_pool[index2].UTF8.bytes);
                k=k+2;
                break;
            case OPCODE_invokestatic:
                var_u2 = (code[k+1] << 8) | code[k+2];
                index1 = constant_pool[var_u2].Methodref.class_index;
                index1 = constant_pool[index1].Class.name_index;
                index2 = constant_pool[var_u2].Methodref.name_and_type_index;
                index2 = constant_pool[index2].NameAndType.name_index;
                printf("invokestatic %d <%s.%s>\n", var_u2, constant_pool[index1].UTF8.bytes, constant_pool[index2].UTF8.bytes);
                k=k+2;
                break;
            case OPCODE_invokeinterface:
                var_u2 = (code[k+1] << 8) | code[k+2];
                printf("invokeinterface %d %d %d\n", var_u2, code[k+3], code[k+4]);
                break;
            case OPCODE_new:
                printf("new %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_newarray:
                printf("newarray %d\n", code[k+1]);
                k++;
                break;
            case OPCODE_anewarray:
                printf("anewarray %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_arraylength:
                printf("arraylength\n");
                break;
            case OPCODE_athrow:
                printf("athrow\n");
                break;
            case OPCODE_checkcast:
                printf("checkcast %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_instanceof:
                printf("instanceof %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_monitorenter:
                printf("monitorenter\n");
                break;
            case OPCODE_monitorexit:
                printf("monitorexit\n");
                break;
            case OPCODE_wide:
                printf("wide\n");
                break;
            case OPCODE_multianewarray:
                printf("multianewarray %d %d %d\n", code[k+1], code[k+2], code[k+3]);
                k=k+3;
                break;
            case OPCODE_ifnull:
                printf("ifnull %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_ifnonnull:
                printf("ifnonnull %d %d\n", code[k+1], code[k+2]);
                k=k+2;
                break;
            case OPCODE_goto_w:
                printf("goto_w %d %d %d %d\n", code[k+1], code[k+2], code[k+3], code[k+4]);
                break;
            case OPCODE_jsr_w:
                printf("jsr_w %d %d %d %d\n", code[k+1], code[k+2], code[k+3], code[k+4]);
                break;
            case OPCODE_breakpoint:
                printf("breakpoint\n");
                break;
        }
    }
}

///Debug (sem testar, não tenho certeza do funcionamento de nada)
///-----------------------------------------------------------------------------------------------------------------------------
void printFrame(Frame *frame){
    printOperandStack(frame->operandStack); /// Funcao para imprimir a pilha de operandos - envia a propria pilha
    //printLocalVariableArray(frame->localVariableArray[0].value->attributes_count, frame->localVariableArray); /// Funcao que imprime o vetor de variaveis locais
    printf("Referencia Constant_pool:\t%d", frame->constant_pool);
    //printf("Referencia Methods:\t\t%d", frame->methods);
    printf("Referencia Handler:\t\t%d", frame->handler);
    printf("Return PC:\t\t%d\n", frame->returnPC);
}

///Muito provavelmente não funciona. Tem que testar ainda
void printOperandStack(structOperandStack ** operandStack){
    structOperandStack *operandStackTop;
    operandStackTop = *operandStack;
    printf("Operand Stack:\n");
    printf("---------------------------------------------------------------------------------------\n");
    while(operandStackTop->next != NULL){
        printf("%d->", operandStackTop->value);
        operandStackTop = operandStackTop->next;
    }
    printf("\n---------------------------------------------------------------------------------------\n");
}

void printLocalVariableArray(u2 attributes_count, LocalVariable *localVariableArray){
    printf("Vetor de variáveis locais:\n");
    printf("---------------------------------------------------------------------------------------\n");
    for(int i=0; i<attributes_count; i++){
        printf("%d ", localVariableArray[i].value);
    }
    printf("\n---------------------------------------------------------------------------------------\n");
}
void printHandler(ClassHandler *handler){
    printf("Class Reference:\t%d\n", handler->classRef);
    printFields(handler->classRef->fields_count, handler->fields);
}

void printFields(u2 fields_count, field_info *fields){
    printf("Fields Vector:\n");
    printf("---------------------------------------------------------------------------------------\n");
    for(int i=0; i<fields_count; i++){
        printf("Name:\t\tcp_info #%d\n", fields[i].name_index);
        printf("Descriptor:\tcp_info #%d\n", fields[i].descriptor_index);
        printf("Access Flags:\t\t0x%04x\n", fields[i].access_flags);
        printf("Attribute count:\t\t%d\n\n", fields[i].attributes_count);
        ///Não estou imprimindo os atributos das fields
    }
    printf("\n---------------------------------------------------------------------------------------\n");
}
///-----------------------------------------------------------------------------------------------------------------------------
///
