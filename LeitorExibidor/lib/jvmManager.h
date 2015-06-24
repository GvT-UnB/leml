#ifndef JVMMANAGER_H_INCLUDED
#define JVMMANAGER_H_INCLUDED
#ifdef JVMMANAGER_SERV
#define EXTERN
#else
#define EXTERN extern
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "classManager.h"
#include "exceptionManager.h"
#include "UserViewer.h"
#include "frameManager.h"

#define MAIN_FOUND          1

#define MAX_CLASSES_ON_HEAP 100

/** \brief Instancia um novo objeto da classe class_file.
 *
 * \param handler ClassHandler* Objeto que será criado.
 * \param numberOfClasses u4* Quantidade de classes instanciadas.
 * \param class_file ClassFile* Classe que será instanciada.
 * \return void
 *
 */
EXTERN void createNewObject(ClassHandler * handler, u4 * numberOfClasses,ClassFile * class_file);

/** \brief Instancia um novo Frame de um metodo da classe class_file e logo em seguida o insere no topo da pilha de Frames.
 *
 * \param handler ClassHandler* Objeto dono do metodo.
 * \param method_index u4 Indice do metodo no methods do class_file.
 * \param curPC u4 PC Corrente.
 * \param frameStackTop StructFrameStack* Referencia para o TOPO da pilha de Frames.
 * \return void
 *
 */
EXTERN void createNewFrame(ClassHandler * handler, u4 method_index, u4 curPC,StructFrameStack *frameStackTop);

/** \brief Coloca o frame no topo da pilha de frames.
 *
 * \param frameStackTop StructFrameStack* Referencia para o corrente topo da pilha de frames.
 * \param frame Frame* referencia para o frame que sea inserido no topo da pilha de frames.
 * \return void
 *
 */
EXTERN void pushFrameStack(StructFrameStack **frameStackTop, Frame * frame);

/** \brief Retira um frame do topo da pilha de frames.
 *
 * \param frameStackTop StructFrameStack* Referencia para o TOPO da pilha de frames.
 * \return Frame* Referencia para o frame retirado do topo da pilha.
 *
 */
EXTERN Frame * popFrameStack(StructFrameStack **frameStackTop);

/** \brief Procura pelo metodo main no ClassHandler, caso encontre o metodo main eh criado o Frame dele e inserido no inicio da pilha de frames.
 *
 * \param handler ClassHandler* Referencia ao Objeto.
 * \param curPC u4 PC corrente.
 * \param frameStackTop StructFrameStack* Referencia para o topo da pilha de frames.
 * \return int
 *
 */
EXTERN int createMainFrame(ClassHandler * handler,u4 curPC,StructFrameStack *frameStackTop);

/** \brief Funcao que efetivamente roda a JVM.
 *
 * \param cur_frame Frame* Frame corrente.
 * \param curPC u4* PC corrente.
 * \param numberOfByteInstruction u1* Referencia para o vetor que armazena a quantidade de bytes que cada instrução utiliza.
 * \return EXTERN void
 *
 */
EXTERN void runJVM(Frame * cur_frame,u4 * curPC, u1 * numberOfByteInstruction, StructFrameStack *frameStackTop);

#endif // FRAMEMANAGER_H_INCLUDED
