#ifndef EXCEPTIONMANAGER_H_INCLUDED
#define EXCEPTIONMANAGER_H_INCLUDED
#ifdef EXCEPTIONMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif



/** \brief Apresenta no terminal a mensagem e o codigo de erro.
 *
 * \param exception_code int O c�digo do erro, todos os codigos de erros est�o listados na tabela de erros
 * \param string_error char* A mensagem de erro que deve ser apresentada no terminal, todas as mensagen de erro est�o listadas na tabela de mensagens
 * \param ... Lista todos os argumentos que a mensgaem de erro possa apresentar
 * \return EXTERN void
 *
 */
EXTERN void throwException(int exception_code, char * string_error, ...);

#endif // EXCEPTIONMANAGER_H_INCLUDED

/**< Tabela de codigos de erros */
#define OPEN_FILE_ERROR         1
#define NOT_BYTECODE_JAVA       2

/**< Tabela de mensagens de erros referentes aos diferentes codigos de erro */
#define OPEN_FILE_ERROR_MSG "Nao foi possivel abrir o arquivo %s\n", *argv
#define NOT_BYTECODE_JAVA_MSG "O arquivo informado nao eh um bytecode java\n"
