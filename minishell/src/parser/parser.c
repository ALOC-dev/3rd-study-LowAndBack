#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//인자를 저장하는 노드. 단어 단위로 받음
typedef struct ArgNode{
    char* arg;                          // 문자열
    struct ArgNode* next;               // 다음 노드를 가르키는 포인터
}ArgNode;                    

//인자 저장 노드들의 연결리스트
typedef struct LinkedList{
    ArgNode* head;                      // head 노드를 가르키는 포인터
    int size;                           // 리스트의 사이즈 (인자 개수)
}List;                        

//keyword 와 인자 저장 노드를 갖고 있는 구조체
typedef struct ParsedCommand{
    char* keyword;                      // 입력된 첫번 째 단어를 저장할 keyword
    List args;                          // 인자 리스트
    struct ParsedCommand* next;         // 여러 줄이 입력되었을 때 첫번 째 줄 -> 두 번째 줄 이렇게 연결해줄 포인터
}ParsedCommand;                

//ParseCommand를 생성,초기화 해주는 함수
ParsedCommand* init_command(){
    ParsedCommand* command = (ParsedCommand*)malloc(sizeof(ParsedCommand));     //comman라는 ParsedCommand 생성
    command->next = NULL;                                                       
    command->args.size = 0;
    command->args.head = NULL;
    command->keyword = NULL;                                                    // List의 head, size를 초기화하고 ParsedCommand의 next와 keyword를 초기화해줌
    return command;            
}

//인자가 저장될 node를 만드는 함수
ArgNode* create_Node(char* e){
    ArgNode* node = (ArgNode*)malloc(sizeof(ArgNode));          // 노드 생성
    node->arg = strdup(e);                                      // node의 arg에 매개변수로 넘어온 e를 복사
    node->next = NULL;                                          // node의 포인터를 NULL로 초기화
    return node;                                                // node를 반환
}

//인자를 저장한 List가 비어있는지 확인하는 함수
int isListEmpty(List* L){
    return L->head == NULL;                     //List의 head가 NULL이면 List는 비어있음
}

void append_Node(List* L, char* e){
    ArgNode* node = create_Node(e);         // node를 생성
    if (isListEmpty(L))                     // List L이 비어있다면
    {
        L->head = node;                     // List의 head가 만들어진 node를 가르킴
        node->next = NULL;                  // node의 next포인터를 NULL로 초기화
        L->size++;                          // List사이즈 ++
    }                               
    else{
        ArgNode* p = L->head;               // head를 가르키는 포인터 p 선언
        while(p->next)
        {
            p = p->next;                    // 반복문을 통해 next를 타고 타고 결국 마지막 노드를 가르킴.
        }
        p->next = node;                     // 마지막 포인터가 node를 가르키도록 
        node->next = NULL;                  // node의 next 포인터를 초기화
        L->size++;                          // List 사이즈 ++
    }
    return;
}

ParsedCommand* parse_input(const char* input){
    ParsedCommand* cmd = init_command();                // cmd라는 ParsedCommand 생성 후 초기화
    char* copy = strdup(input);                         // copy에 input 복사
    char* token = strtok(copy, " ");                    // copy를 " " 단위로 쪼개어 token에 저장

    if (token)      
    {
        cmd->keyword = strdup(token);                   // 첫 번째 단어는 keyword에 복사하여 저장
    }
    while((token=strtok(NULL," ")) != NULL ){           // 마지막까지 반복
        if (strcmp(token, cmd->keyword)==0)             
        {
            continue;                                   // 첫 번째 단어는 keyword이므로 args에 저장하지 않고 건너뜀
        }
        append_Node(&cmd->args, token);                 // 나머지 token은 args에 저장
    }
    free(copy);                                         // copy 메모리 해제
    
    return cmd;                                         // cmd 반환
}