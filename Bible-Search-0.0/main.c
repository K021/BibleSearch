//
//  main.c
//  Bible-Search-0.0
//
//  Created by 김 주언 on 15. 1. 9..
//  Copyright (c) 2015년 김 주언. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KO 1
#define EN 2

typedef struct WORD {
    char word[30];
}WORD; // 검색어 구조체

typedef struct LINE{
    int *l;
    int num;
}LINE; // 검색된 라인 배열과 라인 수를 멤버로 갖는 구조체

LINE * search(FILE *file, char *word, LINE *line);
LINE * netline_p(LINE *line_p,int wonum_p);
LINE * netline_pm(LINE *netline_p, LINE *line_m, int wonum_m);
int print_line(FILE *file, LINE *netline);
int print_line_both(FILE *file01, FILE *file02, LINE *netline);
int get_min_index(LINE *line_p, int wonum);

int main(void)
{
    FILE *KoBible =fopen("KoreanBible.txt", "r");
    FILE *EnBible =fopen("EnglishBible.txt", "r");
    
    int will=0; // 의사결정 변수
    int book=0; // 성경 선택 변수
 
    WORD *word_p = (WORD*)malloc(sizeof(WORD)); // +검색어를 저장할 변수
    WORD *word_m = (WORD*)malloc(sizeof(WORD)); // -검색어를 저장할 변수 
    int wonum_p = 1; // +검색어 수
    int wonum_m = 0; // -검색어 수
    
    LINE *line_p = (LINE*)malloc(sizeof(LINE));// + 검색된 라인과 라인 수 구조체
    {
        line_p->l = (int*)malloc(sizeof(int));
        line_p->num = 0;
    }
    LINE *line_m = (LINE*)malloc(sizeof(LINE));// - 검색된 라인과 라인 수 구조체
    {
        line_m->l = (int*)malloc(sizeof(int));
        line_m->num = 0;
    }
    LINE *lastline = (LINE*)malloc(sizeof(LINE));
    {
        lastline->l = (int*)malloc(sizeof(int));
        lastline->num = 0;
    }
    
    
    if(KoBible==NULL||EnBible==NULL) // 성경 파일이 없을 경우
    {
        puts("성구 검색용 파일이 없습니다.");
        puts("성구 검색을 위해서는 \"KoreanBible.txt\" 과 \"EnglishBible.txt\" 파일이 프로그램 디렉터리 안에 있어야 합니다. ");
        return 1;
    }
    
    puts("=================================================="); //50
    puts("                 [성구 검색 프로그램]                 ");
    puts("--------------------------------------------------"); //50
    puts("* 검색어 입력 법 ");
    puts(" : 검색어는 +검색어와 -검색어가 있습니다.");
    puts("   +검색어는 모두 포함되고, -검색어는 모두 제외됩니다. ");
    puts(" : 한국어와 영어 모두 검색 가능합니다.");
    puts(" : 검색되는 성경 텍스트 파일이 불완전한 경우 완전히 검색되지 않을");
    puts("   수도 있습니다.");
    puts("=================================================="); //50
    puts("한국어 성경 검색은 1, 영어 성경 검색은 2 를 입력하세요.");
    printf(":");
    scanf("%d",&book);
    
    if(book==KO)
    {
        puts("한국어 성경 검색입니다. 검색어를 입력하세요.");
    }
    else if(book==EN)
    {
        puts("영어 성경 검색입니다. 검색어를 입력하세요.");
    }

    // < +검색어 입력 반복문 >
    do {
        printf("+검색어:");
        char a = 0;
        gets(&a);
        gets(word_p[wonum_p-1].word);
       // scanf("%s",word_p[wonum_p-1].word); // 검색어 입력
        
        puts("+검색어를 추가하시겠습니까? (네: 1, 아니오: 2)");
        scanf("%d",&will);
        
        if(will==2)
            break;
        
        wonum_p++; // 검색어 수 저장
        word_p = (WORD*)realloc(word_p,wonum_p*sizeof(WORD));
    } while (will==1);
    
    
    // < -검색어 입력 반복문 >
    do {
        puts("-검색어를 추가하시겠습니까? (네: 1, 아니오: 2)");
        scanf("%d",&will);
        
        if(will==2)
            break;
        
        wonum_m++; // 검색어 수 저장
        word_m = (WORD*)realloc(word_m,wonum_m*sizeof(WORD));
        
        puts("-검색어:");
        scanf("%s",word_m[wonum_m-1].word); // 검색어 입력
    } while (will==1);
    
    line_p = (LINE*)realloc(line_p,wonum_p*sizeof(LINE));
    line_m = (LINE*)realloc(line_m,wonum_m*sizeof(LINE));
    
    if(book==KO)
    {
        for (int i=0; i<wonum_p; i++)
        {
            line_p[i] = *search(KoBible, word_p[i].word, &line_p[i]);
        }
        for (int i=0; i<wonum_m; i++)
        {
            line_m[i] = *search(KoBible, word_m[i].word, &line_m[i]);
        }
    }
    else if (book==EN)
    {
        for (int i=0; i<wonum_p; i++)
        {
            line_p[i] = *search(EnBible, word_p[i].word, &line_p[i]);
        }
        for (int i=0; i<wonum_m; i++)
        {
            line_m[i] = *search(EnBible, word_m[i].word, &line_m[i]);
        }
    }

    
    lastline = netline_p(line_p, wonum_p);
    lastline = netline_pm(lastline, line_m, wonum_m);
    
    if(book==KO)
    {
        printf("\n");
        printf("%3d개 구절\n",lastline->num);
        print_line_both(KoBible, EnBible, lastline);
    }
    else if (book==EN)
    {
        printf("\n");
        printf("%3d개 구절\n",lastline->num);
        print_line_both(KoBible, EnBible, lastline);
        printf("\n");
        print_line(KoBible, lastline);
    }

    
    fclose(KoBible);
    fclose(EnBible);
    return 0;
}

LINE * search(FILE *file, char *word, LINE *line) // 검색함수
{
    char buffer[600]; // 텍스트 라인 저장 버퍼
    int line_num = 0; // 라인 번호
    int num = 0; // 검색 라인 갯수
    
    fseek(file,0,0);
    
    while (fgets(buffer, 600 , file) != NULL) // 한 줄씩 버퍼에 저장.
    {
        line_num++;
        
        if(strstr(buffer,word)!=NULL) // 버퍼와 검색어 비교
        {
            num++;
            line->l=(int*)realloc(line->l,num*sizeof(int)); // 검색어 라인 저장.
            line->l[num-1] = line_num;
        }
    }
    line->num = num;
    return line; // 검색어 라인 배열 반환
}

LINE * netline_p(LINE *line_p,int wonum_p)
{
    int num = 0;
    int AND = 1;
    int *searchIndex = (int*)calloc(wonum_p,sizeof(int));
    
    LINE *netline = (LINE*)malloc(sizeof(LINE));
    {
        netline->l = (int*)malloc(sizeof(int));
        netline->num = 0;
    }
    
    int min_index = 0;
    
    min_index = get_min_index(line_p, wonum_p);
    

    for(int i=0; i<line_p[min_index].num; i++)
    {
        AND = 1;
        for(int j=0; j<wonum_p; j++)
        {
            if(j==min_index)
            {
                continue;
            }
            for(int k=searchIndex[j]; k<line_p[j].num; k++)
            {
                if(line_p[min_index].l[i] == line_p[j].l[k])
                {
                    AND++;
                    line_p[j].l[k] = 0;
                    searchIndex[j] = k+1;
                    break;
                }
                else if (line_p[min_index].l[i] < line_p[j].l[k])
                {
                    searchIndex[j] = k;
                    goto next_p;
                }
            }
        }
        if(AND==wonum_p)
        {
            num++;
            netline->l = (int*)realloc(netline->l,num*sizeof(int));
            netline->l[num-1] = line_p[min_index].l[i];
            netline->num = num;
        }
    next_p:
        continue;
    }
    
    
    return netline;
}

LINE * netline_pm(LINE *netline_p, LINE *line_m, int wonum_m)
{
    for(int i=0; i < netline_p->num; i++)
    {
        for(int j=0; j<wonum_m; j++)
        {
            for(int k=0; k<line_m[j].num; k++)
            {
                if(netline_p->l[i]==line_m[j].l[k])
                {
                    netline_p->l[i] = 0;
                    netline_p->num--;
                    goto next_pm;
                }
                else if(netline_p->l[i]<line_m[j].l[k])
                {
                    break;
                }
            }
        }
    next_pm:
        continue;
    }
    
    return netline_p;
}

int print_line(FILE *file, LINE *netline)
{
    
    char buffer[600];
    int line_num = 0;
    int num = 0;
    
    for(int i=0; i < netline->num; i++)
    {
        if(netline->l[i]!=0)
        {
            fseek(file, 0, 0);
            line_num = 0;
            while(fgets(buffer,600,file)!=NULL)
            {
                line_num++;
                if(netline->l[i]==line_num)
                {
                    num++;
                    printf("%03d %s",num,buffer);
                }
            }
        }
    
    }
    if(num==0)
    {
        puts("검색 결과 성경에 없는 단어입니다.");
    }
    return 0;
}

int print_line_both(FILE *file01, FILE *file02, LINE *netline)
{
    
    char buffer[600];
    int line_num = 0;
    int num = 0;
    
    for(int i=0; i < netline->num; i++)
    {
        if(netline->l[i]!=0)
        {
            fseek(file01, 0, 0);
            line_num = 0;
            while(fgets(buffer,600,file01)!=NULL)
            {
                line_num++;
                if(netline->l[i]==line_num)
                {
                    num++;
                    printf("%s",buffer);
                    break;
                }
            }
            fseek(file02, 0, 0);
            line_num = 0;
            while(fgets(buffer,600,file02)!=NULL)
            {
                line_num++;
                if(netline->l[i]==line_num)
                {
                    printf("%s",buffer);
                    printf("\n");
                    break;
                }
            }
        }
        
    }
    if(num==0)
    {
        puts("검색 결과 성경에 없는 단어입니다.");
    }
    return 0;
}

int get_min_index(LINE *line_p, int wonum)
{
    int min = line_p[0].num;
    int min_index = 0;
    
    for(int i=1; i<wonum; i++)
    {
        if(line_p[i].num < min)
        {
            min = line_p[i].num;
            min_index = i;
        }
    }
    return min_index;
}

// 저녁과
// 하나님 창공
// 믿음 큰 낙타 삼
// 너 내 안 거 교제 err
// 교회  더러운 말 성령 소멸


/*
 LINE * netline_p(LINE *line_p,int wonum_p)
 {
 int num = 0;
 int AND = 1;
 LINE *netline = (LINE*)malloc(sizeof(LINE));
 {
 netline->l = (int*)malloc(sizeof(int));
 netline->num = 0;
 }
 
 for(int i=0; i<wonum_p; i++)
 {
 for(int j=0; j<line_p[i].num; j++)
 {
 for(int k=0; k<wonum_p; k++)
 {
 if(k==i)
 {
 continue;
 }
 for(int l=0; l<line_p[k].num; l++)
 {
 if(line_p[i].l[j]==line_p[k].l[l])
 {
 AND++;
 line_p[k].l[l] = 0;
 break;
 }
 }
 }
 if(AND==wonum_p)
 {
 num++;
 netline->l = (int*)realloc(netline->l,num*sizeof(int));
 netline->l[num-1] = line_p[i].l[j];
 netline->num = num;
 }
 }
 }
 
 return netline;
 }

 
 */



// 정욕 억






