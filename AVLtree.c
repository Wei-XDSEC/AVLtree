#include<stdio.h>
#include<stdlib.h>
int hello_error = 0;
//以内是二叉树使用函数及结构体变量和指针的声明----------------------------------------------------------------
//数据 学号 名字 性别 年龄 身高 体重
typedef struct item
{
    long long int index;
    char name[20];
    char sex[10];
    int age;
    float height;
    float weight;
} Item;
//AVL二叉树
typedef struct AVLtreenode
{
    Item item;
    struct AVLtreenode *left;
    struct AVLtreenode *right;
    int height;
} AVLTreeNode;
//强调节点
typedef AVLTreeNode * AVLTree;
//强调位置
typedef AVLTreeNode * Position;
/*操作：    初始化一个AVL树*/
/*后置条件： AVL树被初始化为空，tree指向根*/
void     AVLTr_Initialize(AVLTree *tree);
/*操作：    将一项插入到树中*/
Position AVLTr_Insert(Item item, AVLTree *tree);
/*操作：    将一项从树中删除*/
/*后置条件： 若该节点为树叶，则返回指向它的节点
             若该节点为根，则返回一个空指针
             其他情况返回位置替代后的节点*/
Position AVLTr_Delete(Item item, AVLTree *tree);
/*操作：    找到对应序号的节点*/
/*后置条件： 若存在则返回指向该节点的指针，否则返回空指针*/
Position AVLTr_find(AVLTree *tree, long long int index);
/*操作：    打印这个树*/
void     AVLTr_ShowStructure(AVLTree *tree);
/*操作：    返回节点的高度*/
static int AVLTR_Height(Position tree);
/*操作：    返回两个整数的最大值*/
static int AVLTR_Max2(int num1, int num2);
/*操作：    左-右双旋*/
static Position AVLTR_DoubleRotateWithLeft(Position K1);
/*操作：    右-左双旋*/
static Position AVLTR_DoubleRotateWithRight(Position K1);
/*操作：    左-左单旋*/
static Position AVLTR_SingleRotateWithLeft(Position K3);
/*操作：    右-右单旋*/
static Position AVLTR_SingleRotateWithRight(Position K3);
/*操作：    供AVLTr_ShowStructure使用，递归求解*/
static void AVLTR_ShowStrc(Position T, int depth);
//以内是二叉树使用函数及结构体变量和指针的声明----------------------------------------------------------------

//以内是程序使用函数的声明------------------------------------------------------------------------------------
void add_student(Item Students,AVLTree *root);
void find_student(AVLTree *root);
void delete_student(Item Students,AVLTree *root);
void readfromfile(Item students, AVLTree *root);
void show_students_inrange(AVLTree *root);
void inrange(long long int num, AVLTree *root);
void gothrough(Position *tree,long long int temp,long long  int num);
void gothroughtofile(Position *tree,long long int temp,long long  int num,FILE *file);
//以内是程序使用函数的声明------------------------------------------------------------------------------------

int main(void)
{
    AVLTree root;
    Item Students;
    AVLTr_Initialize(&root);
    char ch = '\0';

    while(1)
    {
        printf("================================================================================================\n");
        printf("Enter a character below to work:\n");
        printf("-A to add students\n");
        printf("-F to find and print\n");
        printf("-P to print the structure\n");
        printf("-D to delete the data\n");
        printf("-R to read from file\n");
        printf("-T to show the root\n");
        printf("-S to sort students in a range\n");
        printf("================================================================================================\n");

        while((ch = getchar()) == '\n' || ch == '\0')
            continue;
        switch (ch)
        {
        case 'A':
            add_student(Students,&root);
            break;
        case 'F':
            find_student(&root);
            break;
        case 'P':
            AVLTr_ShowStructure(&root);
            break;
        case 'D':
            delete_student(Students,&root);
            break;
        case 'R':
            readfromfile(Students, &root);
            break;
        case 'T':
            if(root->item.index == -1 || root == NULL)
                printf("No data\n");
            else
                printf("name:%s<%I64d>,Height:%d\n", root->item.name,root->item.index,root->height);
            break;
        case 'S':
            show_students_inrange(&root);
            break;
        default :
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}



void AVLTr_Initialize(AVLTree *tree)
{
    //分配内存
    (*tree) = (AVLTree)malloc(sizeof(AVLTreeNode));
    if(!tree)
    {
        printf("out of memory!");
        exit(EXIT_FAILURE);
    }
    //初始化根
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    (*tree)->item.name[0] = '\0';
    (*tree)->item.index = -1;
}

Position AVLTr_Insert(Item item, AVLTree *tree)
{
    //函数采用递归法
    if((*tree) == NULL)
    {
        (*tree) = (AVLTreeNode *)malloc(sizeof(AVLTreeNode));
        if(!tree)
        {
            printf("out of memory!");
            exit(EXIT_FAILURE);
        }
        else
        {
            (*tree)->height = 0;
            (*tree)->item = item;
            (*tree)->left = (*tree)->right = NULL;
        }
    }
    //如果节点为根的话
    else if((*tree)->item.index == -1)
        (*tree)->item = item;
    //要插在右边的话
    else if(item.index > (*tree)->item.index)
    {
        //递归
        (*tree)->right = AVLTr_Insert(item, &((*tree)->right));
        //判断是否平衡
        if(AVLTR_Height((*tree)->right) - AVLTR_Height((*tree)->left) == 2)
        {
            if(item.index > (*tree)->right->item.index)
                //如果是右-右的话
                (*tree) = AVLTR_SingleRotateWithRight(*tree);
            else
                //如果是右-左的话
                (*tree) = AVLTR_DoubleRotateWithRight(*tree);
        }
    }
    //要插在左边的话
    else if(item.index < (*tree)->item.index)
    {
        //递归
        (*tree)->left = AVLTr_Insert(item, &((*tree)->left));
        //判断是否平衡
        if(AVLTR_Height((*tree)->left) - AVLTR_Height((*tree)->right) == 2)
        {
            if(item.index < (*tree)->left->item.index)
                //如果是左-左的话
                (*tree) = AVLTR_SingleRotateWithLeft(*tree);
            else
                //如果是左-右的话
                (*tree) = AVLTR_DoubleRotateWithLeft(*tree);
        }
    }
    //更新节点的高度
    (*tree)->height = AVLTR_Max2(AVLTR_Height((*tree)->left), AVLTR_Height((*tree)->right)) + 1;
    return *tree;
}

Position AVLTr_Delete(Item item, AVLTree *tree)
{
    Position T,T1,Ttemp;
    //为空指针的话
    if(*tree == NULL)
    {
        printf("Can't find it\n");
        *tree = NULL;
    }
    //要删除右边项的话
    else if(item.index > (*tree)->item.index)
    {
        //递归
        (*tree)->right = AVLTr_Delete(item,&((*tree)->right));
        //判断可能进行的删除后该节点对应的子树是否平衡
        if(AVLTR_Height((*tree)->left) - AVLTR_Height((*tree)->right) == 2)
        {
            if((*tree)->left->right == NULL)
                //等同于左-左的情况
                *tree = AVLTR_SingleRotateWithLeft(*tree);
            else
            {
                //让该节点->left的左右的高度都符合要求（该节点左右高度差<2）
                //因为(*tree)->left->right == NULL,所以一定符合左-右的要求，先将该节点->left的right降高度
                *tree = AVLTR_DoubleRotateWithLeft(*tree);
                //判断该节点->left的left是否需要降级(必须要判断，否则会产生错误)
                if(AVLTR_Height((*tree)->left->left) - AVLTR_Height((*tree)->left->right) == 2)
                    //单旋降级
                    ((*tree)->left) = AVLTR_SingleRotateWithLeft((*tree)->left);
            }
        }
    }
    //要删除左边项的话
    else if(item.index < (*tree)->item.index)
    {
        //递归
        (*tree)->left = AVLTr_Delete(item,&((*tree)->left));
        //判断可能进行的删除后该节点对应的子树是否平衡
        if(AVLTR_Height((*tree)->right) - AVLTR_Height((*tree)->left) == 2)
        {
            if((*tree)->right->left == NULL)
                //等同于右-右的情况
                *tree = AVLTR_SingleRotateWithRight(*tree);
            else
            {
                //让该节点->right的左右的高度都符合要求（该节点左右高度差<2）
                //因为(*tree)->right->left == NULL,所以一定符合右-左的要求，先将该节点->right的left降高度
                *tree = AVLTR_DoubleRotateWithRight(*tree);
                //判断该节点->right的right是否需要降级(必须要判断，否则会产生错误)
                if(AVLTR_Height((*tree)->right->right) - AVLTR_Height((*tree)->right->left) == 2)
                    ((*tree)->right) = AVLTR_SingleRotateWithLeft((*tree)->right);
            }
        }
    }
    //接下来就比较简单了
    else if((*tree)->item.index == item.index)
    {
        //只要该节点左右之中只有一个树叶，就可以直接拿树叶来替代这个要删除的节点
        if(((*tree)->right != NULL) ^ ((*tree)->left != NULL))
        {
            if((*tree)->left != NULL)
            //该节点的左树叶替换该节点
            {
                T = (*tree)->left;
                free(*tree);
                *tree = T;
            }
            //该节点的右树叶替换该节点
            else
            {
                T = (*tree)->right;
                free(*tree);
                *tree = T;
            }
        }
        //如果这个节点没有树叶的话，直接删除
        else if((*tree)->right == NULL && (*tree)->left == NULL)
        {
            free(*tree);
            *tree = NULL;
        }
        else
        {
            //分配临时的节点来保存将替换该节点的树叶的item
            T = (AVLTreeNode *)malloc(sizeof(AVLTreeNode));
            T1 = (*tree)->right->left;
            //从该节点右节点的左子树找一片树叶替换这个节点（也可以从该节点的左节点的右子树上找树叶，因为该树是AVL树，不用担心不平衡的问题）
            //如果为空的话就拿右节点替换这个节点
            if(T1 == NULL)
            {
                (*tree)->right->left = (*tree)->left;
                T1 = (*tree)->right;
                free(*tree);
                (*tree) = T1;
            }
            //不为空的话就寻找树叶
            else
            {
                Ttemp = (*tree)->right;
                while(T1->left != NULL || T1->right != NULL)
                {
                    Ttemp = T1;
                    (T1->left == NULL)?(T1= T1->right):(T1 = T1->left);
                }
                T->item = T1->item;
                //用递归delete这个树叶便可以不用但心更新高度的问题了
                AVLTr_Delete(T->item,tree);
                //将树中指向这个树叶的指针设为NULL
                (Ttemp->item.index > T1->item.index)?(Ttemp->left = NULL):(Ttemp->right = NULL);
                (*tree)->item = T->item;
                free(T);
            }
        }
    }
    if(*tree != NULL)
        (*tree)->height = AVLTR_Max2(AVLTR_Height((*tree)->left),AVLTR_Height((*tree)->right)) + 1;
    return *tree;
}

//比较简单就不说了
Position AVLTr_find(AVLTree *tree, long long int index)
{
    Position T = *tree;
    if(T == NULL || T->item.index == -1)
        return NULL;
    else if(T->item.index == index)
        return T;
    else if(T->item.index < index)
        return AVLTr_find(&(T->right), index);
    else
        return AVLTr_find(&(T->left), index);
}
//比较简单就不说了
void     AVLTr_ShowStructure(AVLTree *tree)
{
    AVLTR_ShowStrc(*tree, 0);
}
//比较简单就不说了
static int AVLTR_Height(Position tree)
{
    if(tree == NULL)
        return -1;
    else
        return tree->height;
}
//比较简单就不说了
static int AVLTR_Max2(int num1, int num2)
{
    if(num1 > num2)
        return num1;
    else
        return num2;
}
//比较简单就不说了
static Position AVLTR_SingleRotateWithLeft(Position K2)
{
    Position K1;

    K1 = K2->left;
    K2->left = K1->right;
    K1->right = K2;

    K1->height = AVLTR_Max2(AVLTR_Height(K1->left), AVLTR_Height(K1->right)) + 1;
    K2->height = AVLTR_Max2(AVLTR_Height(K2->left), AVLTR_Height(K2->right)) + 1;

    return K1;
}
//比较简单就不说了
static Position AVLTR_SingleRotateWithRight(Position K2)
{
    Position K1;
    K1 = K2->right;
    K2->right = K1->left;
    K1->left = K2;

    K1->height = AVLTR_Max2(AVLTR_Height(K1->left), AVLTR_Height(K1->right)) + 1;
    K2->height = AVLTR_Max2(AVLTR_Height(K2->left), AVLTR_Height(K2->right)) + 1;

    return K1;
}
//比较简单就不说了
static Position AVLTR_DoubleRotateWithLeft(Position K3)
{
    K3->left = AVLTR_SingleRotateWithRight(K3->left);
    return AVLTR_SingleRotateWithLeft(K3);
}
//比较简单就不说了
static Position AVLTR_DoubleRotateWithRight(Position K3)
{
    K3->right = AVLTR_SingleRotateWithLeft(K3->right);
    return AVLTR_SingleRotateWithRight(K3);
}
//比较简单就不说了
static void AVLTR_ShowStrc(Position T, int depth)
{
    int i = depth;

    if(T == NULL)
        ;
    else if(T->item.index == -1)
        printf("No data\n");
    else
    {
        while(i-- > 0)
            printf("-");
        printf("%s<%I64d>\n", T->item.name, T->item.index);
        AVLTR_ShowStrc(T->right, depth+1);
        AVLTR_ShowStrc(T->left, depth+1);
    }

}

void add_student(Item Students,AVLTree *root)
{
    long long int num;
    while(1)
    {
        printf("Enter the 1 to insert,enter -1 to quit\n");
        if(scanf("%I64d", &num) == 0 || num == -1)
            break;
        printf("Enter the index:\n");
        scanf("%I64d", &Students.index);
        printf("Enter the name:\n");
        scanf("%s", Students.name);
        printf("Enter the sex:\n");
        scanf("%s", Students.sex);
        printf("Enter the age:\n");
        scanf("%d", &Students.age);
        printf("Enter the height:\n");
        scanf("%f", &Students.height);
        printf("Enter the weight:\n");
        scanf("%f", &Students.weight);
        AVLTr_Insert(Students, root);
    }
}

void find_student(AVLTree *root)
{
    long long  int num;
    Position T;
    while(1)
    {
        printf("Enter the students,enter -1 to quit\n");
        if(scanf("%I64d", &num) == 0 || num == -1)
            break;
        T = AVLTr_find(root, num);
        if(T == NULL)
            printf("-Can't find it\n");
        else
        {
            printf("----------------------------------------------\n");
            printf("Name:%s Index:%I64d Height:%d\n", T->item.name, T->item.index, T->height);
            printf("Its left: Name:%s Index:%I64d Height:%d\n", (T->left == NULL)?("NULL"):(T->left->item.name),
                    (T->left == NULL)?(-1):(T->left->item.index),(T->left == NULL)?(-1):(T->left->height));
            printf("Its right: Name:%s Index:%I64d Height:%d\n", (T->right == NULL)?("NULL"):(T->right->item.name),
                    (T->right == NULL)?(-1):(T->right->item.index),(T->right == NULL)?(-1):(T->right->height));
            printf("----------------------------------------------\n");
        }
    }
}

void delete_student(Item Students,AVLTree *root)
{
    long long num;
    while(1)
    {
        printf("Enter the students to delete,enter -1 to quit\n");
        if(scanf("%I64d", &num) == 0 || num == -1)
            break;
        Students.index = num;
        AVLTr_Delete(Students, root);
    }
}

void readfromfile(Item Students, AVLTree *root)
{
    FILE *fp;
    if((fp = fopen("D:\\data.txt", "r")) == NULL)
    {
        printf("Can't open it!\n");
        fclose(fp);
    }
    else
    {
        int count = 0;
        while(fscanf(fp, "%I64d %s %s %d %f %f", &Students.index, Students.name, Students.sex
                        ,&Students.age,&Students.height,&Students.weight) == 6)
            {
                count ++;
                AVLTr_Insert(Students, root);
            }
        printf("Success in reading %d data\n", count);
        fclose(fp);
    }
}

void show_students_inrange(AVLTree *root)
{
    long long int grade,major,classes;
    printf("Please enter the grade(17-14):\n");
    if(scanf("%I64d",&grade) != 1)
    {
        printf("Error input!\n");
        return;
    }
    if(14 > grade || grade > 17)
    {
        printf("Out of range!\n");
        return;
    }


    printf("Do you wish to limit the school(1-18)? and 0 to cancel\n");
    scanf("%I64d", &major);
    if(major  ==  0)
    {
        inrange(grade*10e8,root);
        return;
    }
    if(1 > major || major > 18)
    {
        printf("Out of range!\n");
        return;
    }

    printf("Do you wish to limit the classes(11,12,21,22,31,32)? and 0 to cancel\n");
    scanf("%I64d", &classes);
    if(classes ==  0)
    {
        inrange(grade*10e8+major*10e6,root);
        return;
    }
    if(classes != 12 && classes != 11 && classes != 22 && classes != 21 && classes != 32 && classes != 31)
    {
        printf("Out of range!\n");
        return;
    }
    inrange(grade*10e8+major*10e6+classes*10e3,root);
}

void inrange(long long int num, AVLTree *root)
{
    Position T = *root;
    int flag = 0;
    long long int temp = (long long int)num/10e3;
    FILE *fp;
    if((temp/100000)*100000 == temp)
        temp = 999999999;
    else if((temp/1000)*1000 == temp)
        temp = 9999999;
    else
        temp = 9999;
    while(flag == 0 && T != NULL)
    {
        if(temp + num < T->item.index)
            T = T->left;
        else if(num > T->item.index)
            T = T->right;
        else
            flag = 1;
    }

    if(flag == 0)
        printf("Error occurs!\n");
    else
    {
        printf("_____________________________________________\n");
        printf("学号       |姓名   |性别|年龄 |身高   |体重  |\n");
        gothrough(&T,temp,num);
        printf("_____________________________________________|\n");
        fp = fopen("D:\\outcome.txt","w");
        gothroughtofile(&T,temp,num,fp);
        fclose(fp);

    }

}

void gothrough(Position *tree,long long int temp,long long  int num)
{
    Position T = *tree;
    if(T->left != NULL)
        gothrough(&(T->left),temp,num);
    if(T->item.index >= num && T->item.index <= num + temp)
        printf("%I64d| %-6s|  %s|   %d|  %.1f|  %.1f|\n", T->item.index,T->item.name,T->item.sex,T->item.age,T->item.height,T->item.weight);
    if(T->right != NULL)
        gothrough(&(T->right),temp,num);

}

void gothroughtofile(Position *tree,long long int temp,long long  int num,FILE *file)
{
    Position T = *tree;
    if(T->left != NULL)
        gothroughtofile(&(T->left),temp,num,file);
    if(T->item.index >= num && T->item.index <= num + temp)
        fprintf(file,"%I64d %-6s %s %d %.1f %.1f\n", T->item.index,T->item.name,T->item.sex,T->item.age,T->item.height,T->item.weight);
    if(T->right != NULL)
        gothroughtofile(&(T->right),temp,num,file);

}
