#ifndef EXCEPTIONMANAGER_H_INCLUDED
#define EXCEPTIONMANAGER_H_INCLUDED
#ifdef EXCEPTIONMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif



/** \brief Apresenta no terminal a mensagem e o codigo de erro.
 *
 * \param exception_code int O codigo do erro, todos os codigos de erros estao listados na tabela de erros
 * \param string_error char* A mensagem de erro que deve ser apresentada no terminal, todas as mensagen de erro estao listadas na tabela de mensagens
 * \param ... Lista todos os argumentos que a mensgaem de erro possa apresentar
 * \return EXTERN void
 *
 */
EXTERN void throwException(int exception_code, char * string_error, ...);

#endif // EXCEPTIONMANAGER_H_INCLUDED

/**< Tabela de codigos de erros */
#define OPEN_FILE_ERROR         1
#define NOT_BYTECODE_JAVA       2
#define NOT_JAVA_2_VERSION      3
#define CLASS_DIFFER_FILE_NAME  4
#define INSUFFICIENT_MEMORY     5
#define POP_IN_A_EMPTY_STACK    6
#define MAIN_NOT_FOUND          7
#define CLASS_NOT_LOADED        8

/**< Tabela de mensagens de erros referentes aos diferentes codigos de erro */
#define OPEN_FILE_ERROR_MSG "Nao foi possivel abrir o arquivo %s\n", path
#define NOT_BYTECODE_JAVA_MSG "O arquivo informado nao eh um bytecode java\n"
#define NOT_JAVA_2_VERSION_MSG "O bytecode informado nao eh compativel com a JVM 1.2\n"
#define CLASS_DIFFER_FILE_NAME_MSG "O arquivo %s nao define a classe principal %s\n", className, getClassName(class_file)
#define INSUFFICIENT_MEMORY_MSG "Memoria insuficiente. A JVM serah abortada.\n"
#define POP_IN_A_EMPTY_STACK_MSG "O programa tentou executar um pop numa pilha vazia (%s).\n", stackName
#define MAIN_NOT_FOUND_MSG "O metodo MAIN nao foi encontrado na classe %s.\n", handler->classRef->constant_pool[ handler->classRef->constant_pool[handler->classRef->this_class].Class.name_index ].UTF8.bytes
#define CLASS_NOT_LOADED_MSG "Nao foi possivel localizar nem carregar a classe principal %s", full_file_name
