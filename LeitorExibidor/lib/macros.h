#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED
#ifdef MACROS_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include <stdint.h>

/*! Assinatura do arquivo .class */
#define U4_MAGIC 0xCAFEBABE

/*! Tamanho de um byte */
#define BYTE_SIZE 8

/*! Tamanho máximo de instruções que a JVM possui */
#define MAX_INSTRUCTIONS 250

/*! Máscara de bits abaixo que especifica permissões de acesso e propriedades da classe, interface, métodos e atributos */
#define ACC_PUBLIC 0x0001 //Declaração pública para CLASSES, METODOS E ATRIBUTOS
#define ACC_PRIVATE 0x0002 //Declaração privada para METODOS E ATRIBUTOS
#define ACC_PROTECTED 0x0004 //Declaração protegida para METODOS E ATRIBUTOS
#define ACC_STATIC 0x0008 //Declaração estática, ATRIBUTO pertence à classe, não à instância!
#define ACC_FINAL 0x0010 //Declaração Final CLASSES, METODOS E ATRIBUTOS
#define ACC_SYNCHRONIZED 0x0020 //Declaração sincronizado para MÉTODOS
#define ACC_VOLATILE 0x0040 //Declaração volátil para ATRIBUTOS
#define ACC_TRANSIENT 0x0080 //Declarada transiente: não pode ser lida ou gravada por um gerente de objetos persistente. (Reserva para uso futuro da Sun)
#define ACC_NATIVE 0x0100 //MÉTODO Nativo: implementado em linguagem não Java (C, C++, Assembly)
#define ACC_SUPER 0x0020 //Declaração Super, relativo à superclasse
#define ACC_INTERFACE 0x0200 //Declaração de interface, não é uma classe
#define ACC_ABSTRACT 0x0400 //Declaração abstrata, CLASSES não podem ser instanciadas e MÉTODOS não possuem definição.
#define ACC_STRICT 0x0800 //MÉTODO Strictfp: utiliza modo de ponto flutuante FP-strict (não normalizado)

/*! Combinações de máscaras de bits abaixo que especifica permissões de acesso e propriedades da classe, interface, métodos e atributos */


/*! Tipos Válidos de Tags (Constants Pool) */
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

/*! Tipos Válidos de Tags (Constants Pool) */
#define ATTRIBUTE_ConstantValue     1
#define ATTRIBUTE_Code              2
#define ATTRIBUTE_Exceptions        3
#define ATTRIBUTE_InnerClasses      4
#define ATTRIBUTE_Syntethic         5
#define ATTRIBUTE_LineNumber        6
#define ATTRIBUTE_LocalVariable     7
#define ATTRIBUTE_SourceFile        8
#define ATTRIBUTE_Unknown           9

/*! Opcodes*/
#define OPCODE_nop  0
#define OPCODE_aconst_null  1
#define OPCODE_iconst_m1  2
#define OPCODE_iconst_0  3
#define OPCODE_iconst_1  4
#define OPCODE_iconst_2  5
#define OPCODE_iconst_3  6
#define OPCODE_iconst_4  7
#define OPCODE_iconst_5  8
#define OPCODE_lconst_0  9
#define OPCODE_lconst_1  10
#define OPCODE_fconst_0  11
#define OPCODE_fconst_1  12
#define OPCODE_fconst_2  13
#define OPCODE_dconst_0  14
#define OPCODE_dconst_1  15
#define OPCODE_bipush  16
#define OPCODE_sipush  17
#define OPCODE_ldc  18
#define OPCODE_ldc_w  19
#define OPCODE_ldc2_w  20
#define OPCODE_iload  21
#define OPCODE_lload  22
#define OPCODE_fload  23
#define OPCODE_dload  24
#define OPCODE_aload  25
#define OPCODE_iload_0  26
#define OPCODE_iload_1  27
#define OPCODE_iload_2  28
#define OPCODE_iload_3  29
#define OPCODE_lload_0  30
#define OPCODE_lload_1  31
#define OPCODE_lload_2  32
#define OPCODE_lload_3  33
#define OPCODE_fload_0  34
#define OPCODE_fload_1  35
#define OPCODE_fload_2  36
#define OPCODE_fload_3  37
#define OPCODE_dload_0  38
#define OPCODE_dload_1  39
#define OPCODE_dload_2  40
#define OPCODE_dload_3  41
#define OPCODE_aload_0  42
#define OPCODE_aload_1  43
#define OPCODE_aload_2  44
#define OPCODE_aload_3  45
#define OPCODE_iaload  46
#define OPCODE_laload  47
#define OPCODE_faload  48
#define OPCODE_daload  49
#define OPCODE_aaload  50
#define OPCODE_baload  51
#define OPCODE_caload  52
#define OPCODE_saload  53
#define OPCODE_istore  54
#define OPCODE_lstore  55
#define OPCODE_fstore  56
#define OPCODE_dstore  57
#define OPCODE_astore  58
#define OPCODE_istore_0  59
#define OPCODE_istore_1  60
#define OPCODE_istore_2  61
#define OPCODE_istore_3  62
#define OPCODE_lstore_0  63
#define OPCODE_lstore_1  64
#define OPCODE_lstore_2  65
#define OPCODE_lstore_3  66
#define OPCODE_fstore_0  67
#define OPCODE_fstore_1  68
#define OPCODE_fstore_2  69
#define OPCODE_fstore_3  70
#define OPCODE_dstore_0  71
#define OPCODE_dstore_1  72
#define OPCODE_dstore_2  73
#define OPCODE_dstore_3  74
#define OPCODE_astore_0  75
#define OPCODE_astore_1  76
#define OPCODE_astore_2  77
#define OPCODE_astore_3  78
#define OPCODE_iastore  79
#define OPCODE_lastore  80
#define OPCODE_fastore  81
#define OPCODE_dastore  82
#define OPCODE_aastore  83
#define OPCODE_bastore  84
#define OPCODE_castore  85
#define OPCODE_sastore  86
#define OPCODE_pop  87
#define OPCODE_pop2  88
#define OPCODE_dup  89
#define OPCODE_dup_x1  90
#define OPCODE_dup_x2  91
#define OPCODE_dup2  92
#define OPCODE_dup2_x1  93
#define OPCODE_dup2_x2  94
#define OPCODE_swap  95
#define OPCODE_iadd  96
#define OPCODE_ladd  97
#define OPCODE_fadd  98
#define OPCODE_dadd  99
#define OPCODE_isub  100
#define OPCODE_lsub  101
#define OPCODE_fsub  102
#define OPCODE_dsub  103
#define OPCODE_imul  104
#define OPCODE_lmul  105
#define OPCODE_fmul  106
#define OPCODE_dmul  107
#define OPCODE_idiv  108
#define OPCODE_ldiv  109
#define OPCODE_fdiv  110
#define OPCODE_ddiv  111
#define OPCODE_irem  112
#define OPCODE_lrem  113
#define OPCODE_frem  114
#define OPCODE_drem  115
#define OPCODE_ineg  116
#define OPCODE_lneg  117
#define OPCODE_fneg  118
#define OPCODE_dneg  119
#define OPCODE_ishl  120
#define OPCODE_lshl  121
#define OPCODE_ishr  122
#define OPCODE_lshr  123
#define OPCODE_iushr  124
#define OPCODE_lushr  125
#define OPCODE_iand  126
#define OPCODE_land  127
#define OPCODE_ior  128
#define OPCODE_lor  129
#define OPCODE_ixor  130
#define OPCODE_lxor  131
#define OPCODE_iinc  132
#define OPCODE_i2l  133
#define OPCODE_i2f  134
#define OPCODE_i2d  135
#define OPCODE_l2i  136
#define OPCODE_l2f  137
#define OPCODE_l2d  138
#define OPCODE_f2i  139
#define OPCODE_f2l  140
#define OPCODE_f2d  141
#define OPCODE_d2i  142
#define OPCODE_d2l  143
#define OPCODE_d2f  144
#define OPCODE_i2b  145
#define OPCODE_i2c  146
#define OPCODE_i2s  147
#define OPCODE_lcmp  148
#define OPCODE_fcmpl  149
#define OPCODE_fcmpg  150
#define OPCODE_dcmpl  151
#define OPCODE_dcmpg  152
#define OPCODE_ifeq  153
#define OPCODE_ifne  154
#define OPCODE_iflt  155
#define OPCODE_ifge  156
#define OPCODE_ifgt  157
#define OPCODE_ifle  158
#define OPCODE_if_icmpeq  159
#define OPCODE_if_icmpne  160
#define OPCODE_if_icmplt  161
#define OPCODE_if_icmpge  162
#define OPCODE_if_icmpgt  163
#define OPCODE_if_icmple  164
#define OPCODE_if_acmpeq  165
#define OPCODE_if_acmpne  166
#define OPCODE_goto  167
#define OPCODE_jsr  168
#define OPCODE_ret  169
#define OPCODE_tableswitch  170
#define OPCODE_lookupswitch  171
#define OPCODE_ireturn  172
#define OPCODE_lreturn  173
#define OPCODE_freturn  174
#define OPCODE_dreturn  175
#define OPCODE_areturn  176
#define OPCODE_return  177
#define OPCODE_getstatic  178
#define OPCODE_putstatic  179
#define OPCODE_getfield  180
#define OPCODE_putfield  181
#define OPCODE_invokevirtual  182
#define OPCODE_invokespecial  183
#define OPCODE_invokestatic  184
#define OPCODE_invokeinterface  185
#define OPCODE_new  187
#define OPCODE_newarray  188
#define OPCODE_anewarray  189
#define OPCODE_arraylength  190
#define OPCODE_athrow  191
#define OPCODE_checkcast  192
#define OPCODE_instanceof  193
#define OPCODE_monitorenter  194
#define OPCODE_monitorexit  195
#define OPCODE_wide  196
#define OPCODE_multianewarray  197
#define OPCODE_ifnull  198
#define OPCODE_ifnonnull  199
#define OPCODE_goto_w  200
#define OPCODE_jsr_w  201
#define OPCODE_breakpoint  202

/*! \typedef  u1
    \brief 1 byte
*/
typedef unsigned char u1; //1 byte
/*! \typedef  u2
    \brief 2 bytes
*/
typedef unsigned short u2; //2 bytes
/*! \typedef  u4
    \brief 4 bytes
*/
typedef int32_t u4; //4 bytes

/*! \typedef  u8
    \brief 8 bytes
*/
typedef int64_t u8; //8 bytes


/*! \union cp_info
    \brief O famoso Constante Pool!
*/
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
            u1 * bytes;
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

/*! \struct   ExceptionTable
    \brief structs relacionadas as estruturas de ExceptionTable do attribute_info
*/
typedef struct {
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
} ExceptionTable;

/*! \struct   Classes
    \brief structs relacionadas as estruturas de Classes do attribute_info
*/
typedef struct {
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;
} Classes;

/*! \struct   Line_number_table
    \brief structs relacionadas as estruturas de Line_number_table do attribute_info
*/
typedef struct {
    u2 start_pc;
    u2 line_number;
} Line_number_table;

/*! \struct   Local_variable_table
    \brief structs relacionadas as estruturas de Local_variable_table do attribute_info
*/
typedef struct {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
} Local_variable_table;

/*! \struct   attribute_info
    \brief structs referente ao campo attribute_info
*/
typedef struct{
    u2 name_index;//Use essa tag para definir a union
	u4 length;
	u1 tag;
	union {
   		struct {
            u2 index;
		} ConstantValue;
		struct {
            u2 max_stack;
            u2 max_locals;
            u4 code_length;
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
        } InnerClasses;
        struct {
            u2 line_number_table_length;
            Line_number_table *Line_number_table;
        } LineNumber;
        struct {
            u2 local_variable_table_length;
            Local_variable_table *Local_variable_table;
        } LocalVariable;
        struct {
            u2 sourcefile_index;
        } SourceFile;
	}
} attribute_info;

/*! \struct   field_info
    \brief structs referente ao campo field_info
*/
typedef struct{
    u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
    attribute_info *attributes;
} field_info;

/*! \struct   method_info
    \brief structs referente ao campo method_info
*/
typedef struct {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info *attributes;
} method_info;

/*! \struct   Field_Value
    \brief structs referente ao array de fields estaticos
*/
typedef struct{
	u2 field_index;
	union {
		struct {
            u4 value;
		} U4;
		struct {
            u4 high;
            u4 low;
		} U8;
	}
}Field_Value;

/*! \struct   ClassFile
    \brief Estrutura Interna dum arquivo .class
*/
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
    method_info *methods;
    u2 attributes_count;
    attribute_info *attributes;

    u2 class_full_name_length;///Quantidade de bytes necessarios para armazenar o nome completo da classe.
    u1 * class_full_name; ///Nome completo da classe, incluindo o endereco ateh ela a patir daparta raiz.
    Field_Value * field_value; ///Array com os fields estaticos
}ClassFile;

/*! \struct   LocalVariable
    \brief structs referente aos items do vetor de variaveis locais do Frame
*/
typedef struct{
    u4 value;
    //u1 local_variable_type;
} LocalVariable;


typedef struct{
    u4 value;   ///Armazena o valor do operando
    struct structOperandStack *next; /// Ponteiro para o próximo no da pilha
}structOperandStack;

/*! \struct   ClassHandler
    \brief structs referente aos dados que sao armazenados quando uma classe eh instanciada (Objeto).
*/
typedef struct{
    ClassFile * classRef; ///Referencia para a classe alocada na memoria
    field_info * fields; ///Espaco de memoria alocado para os fields do Objeto
}ClassHandler; ///Objeto

typedef struct{
    structOperandStack * operandStack; ///Pilha de Operandos
    LocalVariable * localVariableArray; /**< O indice ZERO do array é uma REFERENCIA para o method_info */
    cp_info * constant_pool;///Referencia para o constant pool da classe
    method_info *methods; ///Referencia para o method_info do metodo dono do frame
    ClassHandler *handler; ///Referencia para o Objeto dono do metodo
    u4 returnPC; ///Endereco do PC atual do metodo que invocou o metodo corrente.
} Frame;

#define FRAME_STACK_MAX     1000
typedef struct StructFrameStack{
    u4 stackTop;
    Frame frame[FRAME_STACK_MAX];
    //struct StructFrameStack *next;
}StructFrameStack;

/**< Definições gerais.  */
#define NOT_RETURN -1
#define ATTRIBUTE_COD_NOT_FOUND -1
#define CLASS_NOT_FOUND     -1

 u1 * rootDirectory; ///Nome do diretorio raiz, tendo como base o path do bytecode passado por linha de comando.

#endif // MACROS_H_INCLUDED
