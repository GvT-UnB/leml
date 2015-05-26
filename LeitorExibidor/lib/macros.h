#include <stdint.h>

/**< Assinatura do arquivo .class */
#define U4_MAGIC 0xCAFEBABE

/**< Tamanho de um byte */
#define BYTE_SIZE 8

/**< M�scara de bits abaixo que especifica permiss�es de acesso e propriedades da classe, interface, m�todos e atributos */
#define ACC_PUBLIC 0x0001 //Declara��o p�blica para CLASSES, METODOS E ATRIBUTOS
#define ACC_PRIVATE 0x0002 //Declara��o privada para METODOS E ATRIBUTOS
#define ACC_PROTECTED 0x0004 //Declara��o protegida para METODOS E ATRIBUTOS
#define ACC_STATIC 0x0008 //Declara��o est�tica, ATRIBUTO pertence � classe, n�o � inst�ncia!
#define ACC_FINAL 0x0010 //Declara��o Final CLASSES, METODOS E ATRIBUTOS
#define ACC_SYNCHRONIZED 0x0020 //Declara��o sincronizado para M�TODOS
#define ACC_VOLATILE 0x0040 //Declara��o vol�til para ATRIBUTOS
#define ACC_TRANSIENT 0x0080 //Declarada transiente: n�o pode ser lida ou gravada por um gerente de objetos persistente. (Reserva para uso futuro da Sun)
#define ACC_NATIVE 0x0100 //M�TODO Nativo: implementado em linguagem n�o Java (C, C++, Assembly)
#define ACC_SUPER 0x0020 //Declara��o Super, relativo � superclasse
#define ACC_INTERFACE 0x0200 //Declara��o de interface, n�o � uma classe
#define ACC_ABSTRACT 0x0400 //Declara��o abstrata, CLASSES n�o podem ser instanciadas e M�TODOS n�o possuem defini��o.
#define ACC_STRICT 0x0800 //M�TODO Strictfp: utiliza modo de ponto flutuante FP-strict (n�o normalizado)

/**< Tipos V�lidos de Tags (Constants Pool) */
#define CONSTANT_Class              7
#define CONSTANT_Fieldref           9
#define CONSTANT_Methodref          10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String             8
#define CONSTANT_Integer            3
#define CONSTANT_Float              4
#define CONSTANT_Long               5
#define CONSTANT_Double             6
#define CONSTANT_NameAndType        12
#define CONSTANT_Utf8               1

/**< Tipos V�lidos de Tags (Constants Pool) */
#define ATTRIBUTE_ConstantValue     1
#define ATTRIBUTE_Code              2
#define ATTRIBUTE_Exceptions        3
#define ATTRIBUTE_InnerClasses      4
#define ATTRIBUTE_Syntethic         5
#define ATTRIBUTE_LineNumber        6
#define ATTRIBUTE_LocalVariable     7
#define ATTRIBUTE_SourceFile        8
#define ATTRIBUTE_Unknown           9

/**<Tipos de Dados do arquivo .class*/
typedef unsigned char u1; //1 byte
typedef unsigned short u2; //2 bytes
typedef int32_t u4; //4 bytes

/**< O famoso Constante Pool! */
typedef struct {
	u1 tag;
	union {
   		struct {
      		u2 name_index;
		} Class; //tag = 7
		struct {
            u2 class_index;
            u2 name_and_type_index;
		} Fieldref; //tag = 9
		struct {
            u2 name_index;
            u2 descriptor_index;
		} NameAndType; //tag = 12
		struct {
            u2 length;
            u1 *bytes;
		}UTF8; //tag = 1
		struct {
            u2 class_index;
            u2 name_and_type_index;
		} Methodref; //tag = 10
		struct {
            u2 class_index;
            u2 name_and_type_index;
		} InterfaceMethodref; //tag = 11
		struct {
            u2 string_index;
		} String; //tag = 8
		struct {
            u4 bytes;
		} Integer; //tag = 3
		struct {
            u4 bytes;
		} Float; //tag = 4
		struct {
            u4 high_bytes;
            u4 low_bytes;
		} Long; //tag = 5
		struct {
            u4 high_bytes;
            u4 low_bytes;
		} Double; //tag = 6
	}
}cp_info;

/**< structs relacionadas as estruturas de atributo incluindo a propria */
typedef struct {
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
} ExceptionTable;

typedef struct {
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;
} Classes;

typedef struct {
    u2 start_pc;
    u2 line_number;
} Line_number_table;

typedef struct {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
} Local_variable_table;

typedef struct{
    u2 name_index;//Use essa tag para definir a union
	u4 length;
	u1 tag;
	union {
   		struct {
            u2 index;
		} Constantvalue;
		struct {
            u2 max_stack;
            u2 max_locals;
            u4 code_length; �
            u1 *code; //code[code_length]
            u2 exception_table_length;
            ExceptionTable *ExceptionTable;
            u2 attributes_count;
            struct attribute_info *attributes;
		} Code;
		struct {
            u2 number_of_exceptions;
            u2 *exception_index_table;
		} Exceptions;
		struct {
            u2 number_of_classes;
            Classes *Classes;
        } innerClasses;
        struct {
            u2 line_number_table_length;
            Line_number_table *Line_number_table;
        } Linenumber;
        struct {
            u2 local_variable_table_length;
            Local_variable_table *Local_variable_table;
        } Localvariable;
        struct {
            u2 sourcefile_index;
        } Sourcefile;
	}
} attribute_info;

typedef struct{
    u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	//attribute_info attributes[attributes_count];
    attribute_info *attributes;
} field_info;

typedef struct {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info *attributes;
} method_info;

/**<Estrutura Interna dum arquivo .class*/
typedef struct{
    u4 magic;
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    cp_info *constant_pool; //IMPORTANTE: O Constant_Pool inicia no indice 1 !!!
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;
    u2 fields_count;
    field_info *fields;
    u2 methods_count;
    //method_info* methods;
    u2 attributes_count;
    //attribute_info *attributes;
}ClassFile;





