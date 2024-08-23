#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 50
#define MAX_GROUP_STRENGTH 5
#define ORDER 5
#define MIN 2

typedef enum
{
    NO,
    YES
} Groupstatus;

typedef enum
{
    FALSE,
    TRUE
} Boolean;

typedef struct user_tag
{
    int ID;
    char name[MAX_LEN];
    int age;
    int dailyGoal;
    int points;
    int stepCount[7];
    Groupstatus inGroup;
    struct user_tag *next;
} User;

typedef struct member_tag
{
    int memberId;
    User *userNode;
    struct member_tag *next;
} Member;

typedef struct group_tag
{
    int groupId;
    char groupName[MAX_LEN];
    Member *MemberDetails;
    int groupGoal;
    int members;
    struct group_tag *next;
} Group;

typedef struct BTree_user
{
    User *data[ORDER];
    struct BTree_user *child[ORDER + 1];
    struct BTree_user *parent;
    int size;
} BTree_user;

typedef struct BTree_group
{
    Group *data[ORDER];
    struct BTree_group *child[ORDER + 1];
    struct BTree_group *parent;
    int size;
} BTree_group;

/////////////////// HELPER FUNCTIONs /////////////////////////

BTree_user *CreateNode_user()
{
    BTree_user *node = (BTree_user *)malloc(sizeof(BTree_user));
    node->size = 0;
    node->parent = NULL;
    int i;
    for (i = 0; i < ORDER; i++)
    {
        node->child[i] = NULL;
        node->data[i] = NULL;
    }
    node->child[i] = NULL;
    return node;
}

BTree_group *CreateNode_group()
{
    BTree_group *node = (BTree_group *)malloc(sizeof(BTree_group));
    node->size = 0;
    node->parent = NULL;
    int i;
    for (i = 0; i < ORDER; i++)
    {
        node->child[i] = NULL;
        node->data[i] = NULL;
    }
    node->child[i] = NULL;
    return node;
}

User *createUser(int id, char Name[], int Age, int goal, int step[])
{
    User *create = (User *)malloc(sizeof(User));
    create->ID = id;
    strcpy((create->name), Name);
    create->age = Age;
    create->dailyGoal = goal;
    for (int i = 0; i < 7; i++)
        (create->stepCount[i]) = step[i];
    create->inGroup = NO;
    create->next = NULL;
    create->points = 0;
}

User *findUser(BTree_user *users, int id)
{
    int found = 0;
    BTree_user *currNode = users;
    User *ret_val = NULL;

    while (!found && currNode)
    {
        int i = 0;
        while (!found && i < currNode->size && currNode->data[i]->ID <= id)
        {
            if (currNode->data[i]->ID == id)
            {
                ret_val = currNode->data[i];
                found = 1;
            }
            else
                i++;
        }

        if (!found)
            currNode = currNode->child[i];
    }
    return ret_val;
}

Group *findGroup(BTree_group *groups, int id)
{
    int found = 0;
    BTree_group *currNode = groups;
    Group *ret_val = NULL;

    while (!found && currNode)
    {
        int i = 0;
        while (!found && i < currNode->size && currNode->data[i]->groupId <= id)
        {
            if (currNode->data[i]->groupId == id)
            {
                ret_val = currNode->data[i];
                found = 1;
            }
            else
                i++;
        }

        if (!found)
            currNode = currNode->child[i];
    }
    return ret_val;
}

void insertMember(Member **head, Member *Node)
{
    if (*head == NULL || (*head)->memberId > Node->memberId)
    {
        Node->next = *head;
        *head = Node;
    }
    else
    {
        Member *curr = *head;
        Member *prev = NULL;
        while (curr && curr->memberId < Node->memberId)
        {
            prev = curr;
            curr = curr->next;
        }
        Node->next = curr;
        prev->next = Node;
    }
}

Group *createGroupNode(int id, char name[], int goal, int noOfMembers)
{
    Group *newGroup = (Group *)malloc(sizeof(Group));
    newGroup->groupId = id;
    strcpy(newGroup->groupName, name);
    newGroup->groupGoal = goal;
    newGroup->members = noOfMembers;
    newGroup->next = NULL;
    newGroup->MemberDetails = NULL;
    return newGroup;
}

void printUser(User *head)
{
    printf("\n\n");
    printf("Name         | %s\n", head->name);
    printf("ID           | %d\n", head->ID);
    printf("Age          | %d\n", head->age);
    printf("Daily Goal   | %d steps\n", head->dailyGoal);
    printf("Points       | %d pts\n", head->points);

    if (head->inGroup)
        printf("In group     | YES\n");
    else
        printf("In group     | NO\n");

    printf("\nWeekly step count analysis: \n");
    for (int i = 0; i < 7; i++)
    {
        if (head->dailyGoal <= head->stepCount[i])
            printf("Day - %d | Step count : %d | Daily Goal Status : Done\n", i + 1, head->stepCount[i]);
        else
            printf("Day - %d | Step count : %d | Daily Goal Status : Not Done\n", i + 1, head->stepCount[i]);
    }
    printf("\n");
}

void printGroup(Group *currGroup)
{
    int i = 1;
    printf("Group ID      |  %d\n", currGroup->groupId);
    printf("Group name    |  %s\n", currGroup->groupName);
    printf("Group goal    |  %d steps\n", currGroup->groupGoal);
    printf("Members: \n");
    Member *currMember = currGroup->MemberDetails;
    while (currMember)
    {
        printf("  %d) %s (%d)\n", i, currMember->userNode->name, currMember->memberId);
        i++;
        currMember = currMember->next;
    }
}

long long int stepcount(User *head)
{
    long long int step = 0;
    for (int i = 0; i < 7; i++)
        step += head->stepCount[i];
    return step;
}

Boolean isConsistent(User *head)
{
    Boolean isconsistent = TRUE;
    int i = 0;
    while (isconsistent && i < 7)
    {
        isconsistent = (isconsistent && (head->stepCount[i] >= head->dailyGoal));
        i++;
    }
    return isconsistent;
}

long long int groupStepcount(Group *node)
{
    Member *head = node->MemberDetails;
    long long int steps = 0;
    while (head)
    {
        long long int t = stepcount(head->userNode);
        steps += t;
        head = head->next;
    }
    return steps;
}

void findGroupinLeaderBoard(BTree_group *root, int id, Group **foundnode)
{
    if (root && (*foundnode == NULL))
    {
        for (int i = 0; i < root->size; i++)
        {
            findGroupinLeaderBoard(root->child[i], id, foundnode);
            if (root->data[i]->groupId == id)
            {
                *foundnode = root->data[i];
            }
        }
        findGroupinLeaderBoard(root->child[root->size], id, foundnode);
    }
}

int findGroupRank(BTree_group *groups, int id, Boolean *foundFirstGroup, int *currRank, int *Rank)
{
    if (groups)
    {
        for (int i = 0; i < groups->size; i++)
        {
            findGroupRank(groups->child[i], id, foundFirstGroup, currRank, Rank);
            if (*foundFirstGroup)
            {
                (*currRank)++;
                if (groups->data[i]->groupId == id)
                    *Rank = *currRank;
            }
        }
        findGroupRank(groups->child[groups->size], id, foundFirstGroup, currRank, Rank);
    }
    else
        *foundFirstGroup = TRUE;
}

void updateUserLeaderBoard(BTree_user **leader, BTree_user **users, int *point, Boolean *found)
{
    if (*leader)
    {
        for (int i = 0; i < (*leader)->size; i++)
        {
            updateUserLeaderBoard(&(*leader)->child[i], users, point, found);
            if (*point >= 50 && *found == TRUE)
            {
                (*leader)->data[i]->points += *point;
                User *user = findUser(*users, (*leader)->data[i]->ID);
                user->points += *point;
                *point -= 25;
            }
            else
                *found = FALSE;
        }
        updateUserLeaderBoard(&(*leader)->child[(*leader)->size], users, point, found);
    }
    else if (*found == FALSE)
        *found = TRUE;
}

Member *createMemberList(Member *memList)
{
    Member *head = NULL, *tail = NULL, *curr = memList;
    while (curr)
    {
        Member *new = (Member *)malloc(sizeof(Member));
        new->memberId = curr->memberId;
        new->userNode = curr->userNode;
        new->next = NULL;

        if (head == NULL)
            head = tail = new;
        else
        {
            tail->next = new;
            tail = new;
        }
        curr = curr->next;
    }
    return head;
}

int insertAtSortedSteps_User(User *data[], User *newUser, int n)
{
    int i = 0;
    int steps = stepcount(newUser);
    while (i < n && stepcount(data[i]) > steps)
        i++;

    for (int j = n - 1; j >= i; j--)
        data[j + 1] = data[j];

    data[i] = newUser;
    return i;
}

int insertAtSortedID_User(User *data[], User *newUser, int n)
{
    int i = 0;
    while (i < n && data[i]->ID < newUser->ID)
        i++;

    for (int j = n - 1; j >= i; j--)
        data[j + 1] = data[j];

    data[i] = newUser;
    return i;
}

int insertAtSortedSteps_Group(Group *data[], Group *newUser, int n)
{
    int i = 0;
    int steps = groupStepcount(newUser);
    while (i < n && groupStepcount(data[i]) > steps)
        i++;

    for (int j = n - 1; j >= i; j--)
        data[j + 1] = data[j];

    data[i] = newUser;
    return i;
}

int insertAtSortedID_Group(Group *data[], Group *newGroup, int n)
{
    int i = 0;
    while (i < n && data[i]->groupId < newGroup->groupId)
        i++;

    for (int j = n - 1; j >= i; j--)
        data[j + 1] = data[j];

    data[i] = newGroup;
    return i;
}

BTree_user *split_user(BTree_user *Node, int flag)
{
    BTree_user *newNode = CreateNode_user();
    int midIndex = ORDER / 2;
    User *midUser = Node->data[midIndex];
    int i;
    int j = 0;
    for (i = midIndex + 1; i < ORDER; i++)
    {
        newNode->data[j] = Node->data[i];
        newNode->child[j] = Node->child[i];

        if (Node->child[i] != NULL)
            newNode->child[j]->parent = newNode;

        Node->data[i] = NULL;
        Node->child[i] = NULL;
        j++;
    }

    newNode->child[j] = Node->child[i];
    newNode->parent = Node->parent;
    newNode->size = midIndex;
    Node->size = midIndex;

    if (Node->parent != NULL)
    {
        int idx;
        if (flag)
            idx = insertAtSortedSteps_User(Node->parent->data, midUser, Node->parent->size);
        else
            idx = insertAtSortedID_User(Node->parent->data, midUser, Node->parent->size);

        for (int k = Node->parent->size; k > idx; k--)
            Node->parent->child[k + 1] = Node->parent->child[k];

        Node->parent->child[idx + 1] = newNode;
        (Node->parent->size)++;
    }
    else
    {
        BTree_user *newRoot = CreateNode_user();
        newRoot->data[0] = midUser;
        newRoot->child[0] = Node;
        newRoot->child[1] = newNode;
        newRoot->parent = NULL;
        newRoot->size = 1;
        newNode->parent = newRoot;
        Node->parent = newRoot;
        Node = newRoot;
    }
    return Node;
}

BTree_group *split_group(BTree_group *Node, int flag)
{
    BTree_group *newNode = CreateNode_group();
    int midIndex = ORDER / 2;
    Group *midGroup = Node->data[midIndex];
    int i;
    int j = 0;
    for (i = midIndex + 1; i < ORDER; i++)
    {
        newNode->data[j] = Node->data[i];
        newNode->child[j] = Node->child[i];

        if (Node->child[i] != NULL)
            newNode->child[j]->parent = newNode;

        Node->data[i] = NULL;
        Node->child[i] = NULL;
        j++;
    }

    newNode->child[j] = Node->child[i];
    newNode->parent = Node->parent;
    newNode->size = midIndex;
    Node->size = midIndex;

    if (Node->parent != NULL)
    {
        int idx;
        if (flag)
            idx = insertAtSortedSteps_Group(Node->parent->data, midGroup, Node->parent->size);
        else
            idx = insertAtSortedID_Group(Node->parent->data, midGroup, Node->parent->size);

        for (int k = Node->parent->size; k > idx; k--)
            Node->parent->child[k + 1] = Node->parent->child[k];

        Node->parent->child[idx + 1] = newNode;
        (Node->parent->size)++;
    }
    else
    {
        BTree_group *newRoot = CreateNode_group();
        newRoot->data[0] = midGroup;
        newRoot->child[0] = Node;
        newRoot->child[1] = newNode;
        newRoot->parent = NULL;
        newRoot->size = 1;
        newNode->parent = newRoot;
        Node->parent = newRoot;
        Node = newRoot;
    }
    return Node;
}

BTree_user *InsertOnSteps_Users(BTree_user *users, User *newUser, BTree_user *prev)
{
    if (users == NULL)
    {
        users = CreateNode_user();
        users->data[0] = newUser;
        users->size++;
        users->parent = prev;
    }
    else
    {
        int i = 0, flag = 1, steps = stepcount(newUser);
        while (flag && i < users->size)
        {
            if (steps > stepcount(users->data[i]))
                flag = 0;
            else
                i++;
        }

        if (users->child[i] != NULL)
            users->child[i] = InsertOnSteps_Users(users->child[i], newUser, users);
        else
        {
            insertAtSortedSteps_User(users->data, newUser, users->size);
            (users->size)++;
        }

        if (users->size >= ORDER)
            users = split_user(users, 1);
    }
    return users;
}

BTree_user *InsertOnID_Users(BTree_user *users, User *newUser, BTree_user *prev)
{
    if (users == NULL)
    {
        users = CreateNode_user();
        users->data[0] = newUser;
        users->size++;
        users->parent = prev;
    }
    else
    {
        int i = 0, flag = 1;
        while (flag && i < users->size)
        {
            if (newUser->ID < users->data[i]->ID)
                flag = 0;
            else
                i++;
        }

        if (users->child[i] != NULL)
            users->child[i] = InsertOnID_Users(users->child[i], newUser, users);
        else
        {
            insertAtSortedID_User(users->data, newUser, users->size);
            (users->size)++;
        }

        if (users->size >= ORDER)
            users = split_user(users, 0);
    }
    return users;
}

BTree_group *InsertOnSteps_Group(BTree_group *groups, Group *newGroup, BTree_group *prev)
{
    if (groups == NULL)
    {
        groups = CreateNode_group();
        groups->data[0] = newGroup;
        groups->size++;
        groups->parent = prev;
    }
    else
    {
        int i = 0, flag = 1;
        long long steps = groupStepcount(newGroup);
        while (flag && i < groups->size)
        {
            if (steps > groupStepcount(groups->data[i]))
                flag = 0;
            else
                i++;
        }

        if (groups->child[i] != NULL)
            groups->child[i] = InsertOnSteps_Group(groups->child[i], newGroup, groups);
        else
        {
            insertAtSortedSteps_Group(groups->data, newGroup, groups->size);
            (groups->size)++;
        }

        if (groups->size >= ORDER)
            groups = split_group(groups, 1);
    }
    return groups;
}

BTree_group *InsertOnID_Group(BTree_group *groups, Group *newGroup, BTree_group *prev)
{
    if (groups == NULL)
    {
        groups = CreateNode_group();
        groups->data[0] = newGroup;
        groups->size++;
        groups->parent = prev;
    }
    else
    {
        int i = 0, flag = 1;
        while (flag && i < groups->size)
        {
            if (newGroup->groupId < groups->data[i]->groupId)
                flag = 0;
            else
                i++;
        }

        if (groups->child[i] != NULL)
            groups->child[i] = InsertOnID_Group(groups->child[i], newGroup, groups);
        else
        {
            insertAtSortedID_Group(groups->data, newGroup, groups->size);
            (groups->size)++;
        }

        if (groups->size >= ORDER)
            groups = split_group(groups, 0);
    }
    return groups;
}

int deleteAtsortedID_User(User *data[], int id, int n)
{
    int ret_val = -1;
    int i = 0;
    while (i < n && data[i]->ID != id)
        i++;
    if (i < n)
    {
        ret_val = i;
        User *freeNode = data[i];
        for (int idx = i; idx < n - 1; idx++)
        {
            data[idx] = data[idx + 1];
        }
        free(freeNode);
    }
    return ret_val;
}

int deleteAtSortedSteps_User(User *data[], long long steps, int n)
{
    int ret_val = -1;
    int i = 0;
    while (i < n && stepcount(data[i]) != steps)
        i++;
    if (i < n)
    {
        ret_val = i;
        User *freeNode = data[i];
        for (int idx = i; idx < n - 1; idx++)
        {
            data[idx] = data[idx + 1];
        }
        free(freeNode);
    }
    return ret_val;
}

Boolean isUserNodeLeaf(BTree_user *root)
{
    Boolean ans = TRUE;
    int i = 0;
    while (ans && i <= root->size)
    {
        ans = ans && (root->child[i++] == NULL);
    }
    return ans;
}

int searchUserOnID(User *data[], int id, int n)
{
    int i = 0;
    while (i < n && data[i]->ID < id)
        i++;

    return i;
}

int searchUserOnSteps(User *data[], long long steps, int n)
{
    int i = 0;
    while (i < n && stepcount(data[i]) > steps)
        i++;
    return i;
}

BTree_user *inorderPredUser(BTree_user *node, int idx)
{
    BTree_user *ret_val = NULL;
    if (node->child[idx] != NULL)
    {
        BTree_user *ptr = node->child[idx];
        while (ptr->child[ptr->size] != NULL)
            ptr = ptr->child[ptr->size];
        ret_val = ptr;
    }
    return ret_val;
}

BTree_user *inorderSuccUser(BTree_user *node, int idx)
{
    BTree_user *ret_val = NULL;
    if (node->child[idx + 1] != NULL)
    {
        BTree_user *ptr = node->child[idx + 1];
        while (ptr->child[0] != NULL)
            ptr = ptr->child[0];
        ret_val = ptr;
    }
    return ret_val;
}

BTree_user *Merge_User_ID(BTree_user *root, int val)
{
    BTree_user *ret_val = root;
    if (root && root->parent && root->size < MIN)
    {
        int idx, leftidx, rightidx;
        idx = searchUserOnID(root->parent->data, val, root->parent->size);
        leftidx = idx - 1;
        rightidx = idx + 1;
        if (leftidx >= 0)
        {
            User *data = root->parent->data[leftidx];
            BTree_user *temp = root->parent->child[leftidx];
            int t = temp->size;
            temp->data[t++] = data;
            for (int i = 0; i < root->size; i++)
            {
                temp->data[t++] = root->data[i];
            }
            int j = 0;
            for (int i = temp->size + 1; i <= t; i++)
            {
                temp->child[i] = root->child[j];
                if (root->child[j] != NULL)
                    root->child[j]->parent = temp;
                j++;
            }
            temp->size = t;
            if (temp->parent->parent)
            {
                for (int i = leftidx; i < root->parent->size - 1; i++)
                {
                    root->parent->data[i] = root->parent->data[i + 1];
                }
                for (int i = idx; i < root->parent->size; i++)
                {
                    root->parent->child[i] = root->parent->child[i + 1];
                }
                (root->parent->size)--;
            }
            else
            {
                temp->parent = NULL;
            }
            if (temp->parent == NULL)
            {
                ret_val = temp;
            }
            else
            {
                ret_val = temp->parent;
            }
        }
        else if (rightidx <= root->parent->size)
        {
            User *data = root->parent->data[idx];
            int t = root->size;
            root->data[t++] = data;

            for (int i = 0; i < root->parent->child[rightidx]->size; i++)
            {
                root->data[t++] = root->parent->child[rightidx]->data[i];
            }
            int j = 0;
            for (int i = root->size + 1; i <= t; i++)
            {
                root->child[i] = root->parent->child[rightidx]->child[j];
                if (root->parent->child[rightidx]->child[j] != NULL)
                    root->parent->child[rightidx]->child[j] = root;
                j++;
            }
            root->size = t;
            if (root->parent->parent)
            {
                for (int i = idx; i < root->parent->size - 1; i++)
                {
                    root->parent->data[i] = root->parent->data[i + 1];
                }
                for (int i = rightidx; i < root->parent->size; i++)
                {
                    root->parent->child[i] = root->parent->child[i + 1];
                }
                (root->parent->size)--;
            }
            else
            {
                root->parent = NULL;
            }
            ret_val = root->parent;
        }
    }
    return ret_val;
}

BTree_user *Merge_User_Steps(BTree_user *root, User *delNode)
{
    BTree_user *ret_val = root;
    long long steps = stepcount(delNode);
    if (root && root->parent && root->size < MIN)
    {
        int idx, leftidx, rightidx;
        idx = searchUserOnSteps(root->parent->data, steps, root->parent->size);
        leftidx = idx - 1;
        rightidx = idx + 1;
        if (leftidx >= 0)
        {
            User *data = root->parent->data[leftidx];
            BTree_user *temp = root->parent->child[leftidx];
            int t = temp->size;
            temp->data[t++] = data;
            for (int i = 0; i < root->size; i++)
            {
                temp->data[t++] = root->data[i];
            }
            int j = 0;
            for (int i = temp->size + 1; i <= t; i++)
            {
                temp->child[i] = root->child[j];
                if (root->child[j] != NULL)
                    root->child[j]->parent = temp;
                j++;
            }
            temp->size = t;
            if (temp->parent->parent)
            {
                for (int i = leftidx; i < root->parent->size - 1; i++)
                {
                    root->parent->data[i] = root->parent->data[i + 1];
                }
                for (int i = idx; i < root->parent->size; i++)
                {
                    root->parent->child[i] = root->parent->child[i + 1];
                }
                (root->parent->size)--;
            }
            else
            {
                temp->parent = NULL;
            }
            if (temp->parent == NULL)
            {
                ret_val = temp;
            }
            else
            {
                ret_val = temp->parent;
            }
        }
        else if (rightidx <= root->parent->size)
        {
            User *data = root->parent->data[idx];
            int t = root->size;
            root->data[t++] = data;

            for (int i = 0; i < root->parent->child[rightidx]->size; i++)
            {
                root->data[t++] = root->parent->child[rightidx]->data[i];
            }
            int j = 0;
            for (int i = root->size + 1; i <= t; i++)
            {
                root->child[i] = root->parent->child[rightidx]->child[j];
                if (root->parent->child[rightidx]->child[j] != NULL)
                    root->parent->child[rightidx]->child[j] = root;
                j++;
            }
            root->size = t;
            if (root->parent->parent)
            {
                for (int i = idx; i < root->parent->size - 1; i++)
                {
                    root->parent->data[i] = root->parent->data[i + 1];
                }
                for (int i = rightidx; i < root->parent->size; i++)
                {
                    root->parent->child[i] = root->parent->child[i + 1];
                }
                (root->parent->size)--;
            }
            else
            {
                root->parent = NULL;
            }
            ret_val = root->parent;
        }
    }
    return ret_val;
}

BTree_user *DeleteID_USER(BTree_user *root, int id)
{
    BTree_user *treeItr = root;
    Boolean doneDeletion = FALSE;
    while (treeItr != NULL && !doneDeletion)
    {
        int i = 0;
        while (i < treeItr->size && treeItr->data[i]->ID < id)
            i++;
        if (i < treeItr->size && treeItr->data[i]->ID == id)
        {
            if (isUserNodeLeaf(treeItr))
            {
                int idx = deleteAtsortedID_User(treeItr->data, id, treeItr->size);
                (treeItr->size)--;
                if (treeItr->size < MIN && treeItr->parent)
                {
                    BTree_user *temp = treeItr->parent;
                    int parentidx = searchUserOnID(temp->data, id, temp->size);
                    if (parentidx - 1 >= 0 && temp->child[parentidx - 1]->size > MIN)
                    {
                        User *data = temp->child[parentidx - 1]->data[temp->child[parentidx - 1]->size - 1];
                        User *x = temp->data[parentidx];
                        temp->data[parentidx] = data;
                        insertAtSortedID_User(treeItr->data, x, treeItr->size);
                        deleteAtsortedID_User(temp->child[parentidx - 1]->data, id, temp->child[parentidx - 1]->size);
                        (treeItr->size)++;
                        (temp->child[parentidx - 1]->size)--;
                    }
                    else if (parentidx + 1 <= temp->size && temp->child[parentidx + 1]->size > MIN)
                    {
                        User *data = temp->child[parentidx + 1]->data[0];
                        User *x = temp->data[parentidx];
                        temp->data[parentidx] = data;
                        insertAtSortedID_User(treeItr->data, x, treeItr->size);
                        deleteAtsortedID_User(temp->child[parentidx + 1]->data, id, temp->child[parentidx + 1]->size);
                        (treeItr->size)++;
                        (temp->child[parentidx + 1]->size)--;
                    }
                    else
                    {
                        treeItr->parent = Merge_User_ID(treeItr, id);
                        BTree_user *ptr = treeItr->parent;
                        BTree_user *prevptr = treeItr;
                        Boolean done = FALSE;
                        while (!done && ptr && ptr->parent && ptr->size < MIN)
                        {
                            ptr->parent = Merge_User_ID(ptr, ptr->data[0]->ID);
                            prevptr = ptr;
                            if (ptr && ptr->parent && ptr->parent->parent == NULL)
                            {
                                root = ptr->parent;
                                done = TRUE;
                            }
                            else
                            {
                                ptr = ptr->parent;
                            }
                        }
                        if (ptr == NULL)
                        {
                            root = prevptr;
                        }
                    }
                }
                doneDeletion = TRUE;
            }
            else
            {
                BTree_user *inPred = inorderPredUser(treeItr, i);
                BTree_user *inSucc = inorderSuccUser(treeItr, i);
                if (inPred && inPred->size > MIN)
                {
                    User *pred = inPred->data[inPred->size - 1];
                    treeItr->data[i] = pred;
                    (inPred->size)--;
                }
                else if (inSucc && inSucc->size > MIN)
                {
                    User *succ = inSucc->data[0];
                    treeItr->data[i] = succ;
                    deleteAtsortedID_User(inSucc->data, succ->ID, inSucc->size);
                    (inSucc->size)--;
                }
                else
                {
                    User *pred = inPred->data[inPred->size - 1];
                    if (pred != NULL)
                    {
                        root = DeleteID_USER(root, pred->ID);
                        treeItr->data[i] = pred;
                    }
                    else
                    {
                        User *succ = inSucc->data[0];
                        root = DeleteID_USER(root, succ->ID);
                        treeItr->data[i] = succ;
                    }
                }
                doneDeletion = TRUE;
            }
        }
        else
        {
            treeItr = treeItr->child[i];
        }
    }
    if (treeItr == NULL)
    {
        printf("\n No User found with ID - %d \n", id);
    }
    return root;
}

BTree_user *DeleteSteps_USER(BTree_user *root, User *delNode)
{
    BTree_user *treeItr = root;
    Boolean doneDeletion = FALSE;
    long long steps = stepcount(delNode);
    while (treeItr != NULL && !doneDeletion)
    {
        int i = 0;
        while (i < treeItr->size && stepcount(treeItr->data[i]) > steps)
            i++;
        if (i < treeItr->size && stepcount(treeItr->data[i]) == steps)
        {
            if (isUserNodeLeaf(treeItr))
            {
                int idx = deleteAtSortedSteps_User(treeItr->data, steps, treeItr->size);
                (treeItr->size)--;
                if (treeItr->size < MIN && treeItr->parent)
                {
                    BTree_user *temp = treeItr->parent;
                    int parentidx = searchUserOnSteps(temp->data, steps, temp->size);
                    if (parentidx - 1 >= 0 && temp->child[parentidx - 1]->size > MIN)
                    {
                        User *data = temp->child[parentidx - 1]->data[temp->child[parentidx - 1]->size - 1];
                        User *x = temp->data[parentidx];
                        temp->data[parentidx] = data;
                        insertAtSortedSteps_User(treeItr->data, x, treeItr->size);
                        deleteAtSortedSteps_User(temp->child[parentidx - 1]->data, steps, temp->child[parentidx - 1]->size);
                        (treeItr->size)++;
                        (temp->child[parentidx - 1]->size)--;
                    }
                    else if (parentidx + 1 <= temp->size && temp->child[parentidx + 1]->size > MIN)
                    {
                        User *data = temp->child[parentidx + 1]->data[0];
                        User *x = temp->data[parentidx];
                        temp->data[parentidx] = data;
                        insertAtSortedSteps_User(treeItr->data, x, treeItr->size);
                        deleteAtSortedSteps_User(temp->child[parentidx + 1]->data, steps, temp->child[parentidx + 1]->size);
                        (treeItr->size)++;
                        (temp->child[parentidx + 1]->size)--;
                    }
                    else
                    {
                        treeItr->parent = Merge_User_Steps(treeItr, delNode);
                        BTree_user *ptr = treeItr->parent;
                        BTree_user *prevptr = treeItr;
                        Boolean done = FALSE;
                        while (!done && ptr && ptr->parent && ptr->size < MIN)
                        {
                            ptr->parent = Merge_User_Steps(ptr, ptr->data[0]);
                            prevptr = ptr;
                            if (ptr && ptr->parent && ptr->parent->parent == NULL)
                            {
                                root = ptr->parent;
                                done = TRUE;
                            }
                            else
                            {
                                ptr = ptr->parent;
                            }
                        }
                        if (ptr == NULL)
                        {
                            root = prevptr;
                        }
                    }
                }
                doneDeletion = TRUE;
            }
            else
            {
                BTree_user *inPred = inorderPredUser(treeItr, i);
                BTree_user *inSucc = inorderSuccUser(treeItr, i);
                if (inPred && inPred->size > MIN)
                {
                    User *pred = inPred->data[inPred->size - 1];
                    treeItr->data[i] = pred;
                    (inPred->size)--;
                }
                else if (inSucc && inSucc->size > MIN)
                {
                    User *succ = inSucc->data[0];
                    treeItr->data[i] = succ;
                    deleteAtSortedSteps_User(inSucc->data, steps, inSucc->size);
                    (inSucc->size)--;
                }
                else
                {
                    User *pred = inPred->data[inPred->size - 1];
                    root = DeleteSteps_USER(root, inPred->data[inPred->size - 1]);
                    treeItr->data[i] = pred;
                }
                doneDeletion = TRUE;
            }
        }
        else
        {
            treeItr = treeItr->child[i];
        }
    }
    if (treeItr == NULL)
    {
        printf("\n No User found with ID - %d \n", delNode->ID);
    }
    return root;
}

int deleteAtsortedID_Group(Group *data[], int id, int n)
{
    int ret_val = -1;
    int i = 0;
    while (i < n && data[i]->groupId != id)
        i++;
    if (i < n)
    {
        ret_val = i;
        Group *freeNode = data[i];
        for (int idx = i; idx < n - 1; idx++)
        {
            data[idx] = data[idx + 1];
        }
        free(freeNode);
    }
    return ret_val;
}

int deleteAtSortedSteps_Group(Group *data[], long long steps, int n)
{
    int ret_val = -1;
    int i = 0;
    while (i < n && groupStepcount(data[i]) != steps)
        i++;
    if (i < n)
    {
        ret_val = i;
        Group *freeNode = data[i];
        for (int idx = i; idx < n - 1; idx++)
        {
            data[idx] = data[idx + 1];
        }
        free(freeNode);
    }
    return ret_val;
}

Boolean isGroupNodeLeaf(BTree_group *root)
{
    Boolean ans = TRUE;
    int i = 0;
    while (ans && i <= root->size)
    {
        ans = ans && (root->child[i++] == NULL);
    }
    return ans;
}

int searchGroupOnID(Group *data[], int id, int n)
{
    int i = 0;
    while (i < n && data[i]->groupId < id)
        i++;

    return i;
}

int searchGroupOnSteps(Group *data[], long long steps, int n)
{
    int i = 0;
    while (i < n && groupStepcount(data[i]) > steps)
        i++;
    return i;
}

BTree_group *inorderPredGroup(BTree_group *node, int idx)
{
    BTree_group *ret_val = NULL;
    if (node->child[idx] != NULL)
    {
        BTree_group *ptr = node->child[idx];
        while (ptr->child[ptr->size] != NULL)
            ptr = ptr->child[ptr->size];
        ret_val = ptr;
    }
    return ret_val;
}

BTree_group *inorderSuccGroup(BTree_group *node, int idx)
{
    BTree_group *ret_val = NULL;
    if (node->child[idx + 1] != NULL)
    {
        BTree_group *ptr = node->child[idx + 1];
        while (ptr->child[0] != NULL)
            ptr = ptr->child[0];
        ret_val = ptr;
    }
    return ret_val;
}

BTree_group *Merge_Group_ID(BTree_group *root, int val)
{
    BTree_group *ret_val = root;
    if (root && root->parent && root->size < MIN)
    {
        int idx, leftidx, rightidx;
        idx = searchGroupOnID(root->parent->data, val, root->parent->size);
        leftidx = idx - 1;
        rightidx = idx + 1;
        if (leftidx >= 0)
        {
            Group *data = root->parent->data[leftidx];
            BTree_group *temp = root->parent->child[leftidx];
            int t = temp->size;
            temp->data[t++] = data;
            for (int i = 0; i < root->size; i++)
            {
                temp->data[t++] = root->data[i];
            }
            int j = 0;
            for (int i = temp->size + 1; i <= t; i++)
            {
                temp->child[i] = root->child[j];
                if (root->child[j] != NULL)
                    root->child[j]->parent = temp;
                j++;
            }
            temp->size = t;
            if (temp->parent->parent)
            {
                for (int i = leftidx; i < root->parent->size - 1; i++)
                {
                    root->parent->data[i] = root->parent->data[i + 1];
                }
                for (int i = idx; i < root->parent->size; i++)
                {
                    root->parent->child[i] = root->parent->child[i + 1];
                }
                (root->parent->size)--;
            }
            else
            {
                temp->parent = NULL;
            }
            if (temp->parent == NULL)
            {
                ret_val = temp;
            }
            else
            {
                ret_val = temp->parent;
            }
        }
        else if (rightidx <= root->parent->size)
        {
            Group *data = root->parent->data[idx];
            int t = root->size;
            root->data[t++] = data;

            for (int i = 0; i < root->parent->child[rightidx]->size; i++)
            {
                root->data[t++] = root->parent->child[rightidx]->data[i];
            }
            int j = 0;
            for (int i = root->size + 1; i <= t; i++)
            {
                root->child[i] = root->parent->child[rightidx]->child[j];
                if (root->parent->child[rightidx]->child[j] != NULL)
                    root->parent->child[rightidx]->child[j] = root;
                j++;
            }
            root->size = t;
            if (root->parent->parent)
            {
                for (int i = idx; i < root->parent->size - 1; i++)
                {
                    root->parent->data[i] = root->parent->data[i + 1];
                }
                for (int i = rightidx; i < root->parent->size; i++)
                {
                    root->parent->child[i] = root->parent->child[i + 1];
                }
                (root->parent->size)--;
            }
            else
            {
                root->parent = NULL;
            }
            ret_val = root->parent;
        }
    }
    return ret_val;
}

BTree_group *Merge_Group_Steps(BTree_group *root, Group *delNode)
{
    BTree_group *ret_val = root;
    long long steps = groupStepcount(delNode);
    if (root && root->parent && root->size < MIN)
    {
        int idx, leftidx, rightidx;
        idx = searchGroupOnSteps(root->parent->data, steps, root->parent->size);
        leftidx = idx - 1;
        rightidx = idx + 1;
        if (leftidx >= 0)
        {
            Group *data = root->parent->data[leftidx];
            BTree_group *temp = root->parent->child[leftidx];
            int t = temp->size;
            temp->data[t++] = data;
            for (int i = 0; i < root->size; i++)
            {
                temp->data[t++] = root->data[i];
            }
            int j = 0;
            for (int i = temp->size + 1; i <= t; i++)
            {
                temp->child[i] = root->child[j];
                if (root->child[j] != NULL)
                    root->child[j]->parent = temp;
                j++;
            }
            temp->size = t;
            if (temp->parent->parent)
            {
                for (int i = leftidx; i < root->parent->size - 1; i++)
                {
                    root->parent->data[i] = root->parent->data[i + 1];
                }
                for (int i = idx; i < root->parent->size; i++)
                {
                    root->parent->child[i] = root->parent->child[i + 1];
                }
                (root->parent->size)--;
            }
            else
            {
                temp->parent = NULL;
            }
            if (temp->parent == NULL)
            {
                ret_val = temp;
            }
            else
            {
                ret_val = temp->parent;
            }
        }
        else if (rightidx <= root->parent->size)
        {
            Group *data = root->parent->data[idx];
            int t = root->size;
            root->data[t++] = data;

            for (int i = 0; i < root->parent->child[rightidx]->size; i++)
            {
                root->data[t++] = root->parent->child[rightidx]->data[i];
            }
            int j = 0;
            for (int i = root->size + 1; i <= t; i++)
            {
                root->child[i] = root->parent->child[rightidx]->child[j];
                if (root->parent->child[rightidx]->child[j] != NULL)
                    root->parent->child[rightidx]->child[j] = root;
                j++;
            }
            root->size = t;
            if (root->parent->parent)
            {
                for (int i = idx; i < root->parent->size - 1; i++)
                {
                    root->parent->data[i] = root->parent->data[i + 1];
                }
                for (int i = rightidx; i < root->parent->size; i++)
                {
                    root->parent->child[i] = root->parent->child[i + 1];
                }
                (root->parent->size)--;
            }
            else
            {
                root->parent = NULL;
            }
            ret_val = root->parent;
        }
    }
    return ret_val;
}

BTree_group *DeleteID_Group(BTree_group *root, int id)
{
    BTree_group *treeItr = root;
    Boolean doneDeletion = FALSE;
    while (treeItr != NULL && !doneDeletion)
    {
        int i = 0;
        while (i < treeItr->size && treeItr->data[i]->groupId < id)
            i++;
        if (i < treeItr->size && treeItr->data[i]->groupId == id)
        {
            if (isGroupNodeLeaf(treeItr))
            {
                int idx = deleteAtsortedID_Group(treeItr->data, id, treeItr->size);
                (treeItr->size)--;
                if (treeItr->size < MIN && treeItr->parent)
                {
                    BTree_group *temp = treeItr->parent;
                    int parentidx = searchGroupOnID(temp->data, id, temp->size);
                    if (parentidx - 1 >= 0 && temp->child[parentidx - 1]->size > MIN)
                    {
                        Group *data = temp->child[parentidx - 1]->data[temp->child[parentidx - 1]->size - 1];
                        Group *x = temp->data[parentidx];
                        temp->data[parentidx] = data;
                        insertAtSortedID_Group(treeItr->data, x, treeItr->size);
                        deleteAtsortedID_Group(temp->child[parentidx - 1]->data, id, temp->child[parentidx - 1]->size);
                        (treeItr->size)++;
                        (temp->child[parentidx - 1]->size)--;
                    }
                    else if (parentidx + 1 <= temp->size && temp->child[parentidx + 1]->size > MIN)
                    {
                        Group *data = temp->child[parentidx + 1]->data[0];
                        Group *x = temp->data[parentidx];
                        temp->data[parentidx] = data;
                        insertAtSortedID_Group(treeItr->data, x, treeItr->size);
                        deleteAtsortedID_Group(temp->child[parentidx + 1]->data, id, temp->child[parentidx + 1]->size);
                        (treeItr->size)++;
                        (temp->child[parentidx + 1]->size)--;
                    }
                    else
                    {
                        treeItr->parent = Merge_Group_ID(treeItr, id);
                        BTree_group *ptr = treeItr->parent;
                        BTree_group *prevptr = treeItr;
                        Boolean done = FALSE;
                        while (!done && ptr && ptr->parent && ptr->size < MIN)
                        {
                            ptr->parent = Merge_Group_ID(ptr, ptr->data[0]->groupId);
                            prevptr = ptr;
                            if (ptr && ptr->parent && ptr->parent->parent == NULL)
                            {
                                root = ptr->parent;
                                done = TRUE;
                            }
                            else
                            {
                                ptr = ptr->parent;
                            }
                        }
                        if (ptr == NULL)
                        {
                            root = prevptr;
                        }
                    }
                }
                doneDeletion = TRUE;
            }
            else
            {
                BTree_group *inPred = inorderPredGroup(treeItr, i);
                BTree_group *inSucc = inorderSuccGroup(treeItr, i);
                if (inPred && inPred->size > MIN)
                {
                    Group *pred = inPred->data[inPred->size - 1];
                    treeItr->data[i] = pred;
                    (inPred->size)--;
                }
                else if (inSucc && inSucc->size > MIN)
                {
                    Group *succ = inSucc->data[0];
                    treeItr->data[i] = succ;
                    deleteAtsortedID_Group(inSucc->data, succ->groupId, inSucc->size);
                    (inSucc->size)--;
                }
                else
                {
                    Group *pred = inPred->data[inPred->size - 1];
                    if (pred != NULL)
                    {
                        root = DeleteID_Group(root, pred->groupId);
                        treeItr->data[i] = pred;
                    }
                    else
                    {
                        Group *succ = inSucc->data[0];
                        root = DeleteID_Group(root, succ->groupId);
                        treeItr->data[i] = succ;
                    }
                }
                doneDeletion = TRUE;
            }
        }
        else
        {
            treeItr = treeItr->child[i];
        }
    }
    if (treeItr == NULL)
    {
        printf("\n NoGroup found with ID - %d \n", id);
    }
    return root;
}

BTree_group *DeleteSteps_Group(BTree_group *root, Group *delNode)
{
    BTree_group *treeItr = root;
    Boolean doneDeletion = FALSE;
    long long steps = groupStepcount(delNode);
    while (treeItr != NULL && !doneDeletion)
    {
        int i = 0;
        while (i < treeItr->size && groupStepcount(treeItr->data[i]) > steps)
            i++;
        if (i < treeItr->size && groupStepcount(treeItr->data[i]) == steps)
        {
            if (isGroupNodeLeaf(treeItr))
            {
                int idx = deleteAtSortedSteps_Group(treeItr->data, steps, treeItr->size);
                (treeItr->size)--;
                if (treeItr->size < MIN && treeItr->parent)
                {
                    BTree_group *temp = treeItr->parent;
                    int parentidx = searchGroupOnSteps(temp->data, steps, temp->size);
                    if (parentidx - 1 >= 0 && temp->child[parentidx - 1]->size > MIN)
                    {
                        Group *data = temp->child[parentidx - 1]->data[temp->child[parentidx - 1]->size - 1];
                        Group *x = temp->data[parentidx];
                        temp->data[parentidx] = data;
                        insertAtSortedSteps_Group(treeItr->data, x, treeItr->size);
                        deleteAtSortedSteps_Group(temp->child[parentidx - 1]->data, steps, temp->child[parentidx - 1]->size);
                        (treeItr->size)++;
                        (temp->child[parentidx - 1]->size)--;
                    }
                    else if (parentidx + 1 <= temp->size && temp->child[parentidx + 1]->size > MIN)
                    {
                        Group *data = temp->child[parentidx + 1]->data[0];
                        Group *x = temp->data[parentidx];
                        temp->data[parentidx] = data;
                        insertAtSortedSteps_Group(treeItr->data, x, treeItr->size);
                        deleteAtSortedSteps_Group(temp->child[parentidx + 1]->data, steps, temp->child[parentidx + 1]->size);
                        (treeItr->size)++;
                        (temp->child[parentidx + 1]->size)--;
                    }
                    else
                    {
                        treeItr->parent = Merge_Group_Steps(treeItr, delNode);
                        BTree_group *ptr = treeItr->parent;
                        BTree_group *prevptr = treeItr;
                        Boolean done = FALSE;
                        while (!done && ptr && ptr->parent && ptr->size < MIN)
                        {
                            ptr->parent = Merge_Group_Steps(ptr, ptr->data[0]);
                            prevptr = ptr;
                            if (ptr && ptr->parent && ptr->parent->parent == NULL)
                            {
                                root = ptr->parent;
                                done = TRUE;
                            }
                            else
                            {
                                ptr = ptr->parent;
                            }
                        }
                        if (ptr == NULL)
                        {
                            root = prevptr;
                        }
                    }
                }
                doneDeletion = TRUE;
            }
            else
            {
                BTree_group *inPred = inorderPredGroup(treeItr, i);
                BTree_group *inSucc = inorderSuccGroup(treeItr, i);
                if (inPred && inPred->size > MIN)
                {
                    Group *pred = inPred->data[inPred->size - 1];
                    treeItr->data[i] = pred;
                    (inPred->size)--;
                }
                else if (inSucc && inSucc->size > MIN)
                {
                    Group *succ = inSucc->data[0];
                    treeItr->data[i] = succ;
                    deleteAtSortedSteps_Group(inSucc->data, steps, inSucc->size);
                    (inSucc->size)--;
                }
                else
                {
                    Group *pred = inPred->data[inPred->size - 1];
                    root = DeleteSteps_Group(root, inPred->data[inPred->size - 1]);
                    treeItr->data[i] = pred;
                }
                doneDeletion = TRUE;
            }
        }
        else
        {
            treeItr = treeItr->child[i];
        }
    }
    if (treeItr == NULL)
    {
        printf("\n No Group found with ID - %d \n", delNode->groupId);
    }
    return root;
}

void setGroupStatus(BTree_user **leaders, int id)
{
    if (*leaders)
    {
        for (int i = 0; i < (*leaders)->size; i++)
        {
            setGroupStatus(&(*leaders)->child[i], id);
            if ((*leaders)->data[i]->ID == id)
                (*leaders)->data[i]->inGroup = YES;
        }
        setGroupStatus(&(*leaders)->child[(*leaders)->size], id);
    }
}

void inorder_user(BTree_user *root)
{
    if (root)
    {
        for (int i = 0; i < root->size; i++)
        {
            inorder_user(root->child[i]);
            printUser(root->data[i]);
        }
        inorder_user(root->child[root->size]);
    }
}

void inorder_group(BTree_group *root)
{
    if (root)
    {
        for (int i = 0; i < root->size; i++)
        {
            inorder_group(root->child[i]);
            printf("\n");
            printGroup(root->data[i]);
        }
        inorder_group(root->child[root->size]);
    }
}

void deleteUserfromGroup(BTree_group *groups, int id, int *flag)
{
    if ((groups != NULL) && (*flag == 0))
    {
        for (int i = 0; i < groups->size; i++)
        {
            deleteUserfromGroup(groups->child[i], id, flag);
            Member *member = groups->data[i]->MemberDetails;
            Member *curr = member;
            Member *prev = NULL;
            Boolean found = FALSE;
            while (curr && !found)
            {
                if (curr->memberId == id)
                    found = TRUE;
                else
                {
                    prev = curr;
                    curr = curr->next;
                }
            }
            if (found)
            {
                if (prev)
                {
                    prev->next = curr->next;
                    free(curr);
                }
                else
                {
                    groups->data[i]->MemberDetails = curr->next;
                    free(curr);
                }
                *flag = 1;
            }
        }
        deleteUserfromGroup(groups->child[groups->size], id, flag);
    }
}

/////////////////// ACTUAL FUNCTIONs /////////////////////////

// a)
void Add_Person(BTree_user **users, BTree_user **userLeaderBoard, User *newUser)
{
    *users = InsertOnID_Users(*users, newUser, NULL);

    if (isConsistent(newUser))
    {
        User *temp = createUser(newUser->ID, newUser->name, newUser->age, newUser->dailyGoal, newUser->stepCount);
        *userLeaderBoard = InsertOnSteps_Users(*userLeaderBoard, temp, NULL);
    }
}

// b)
void Create_group(BTree_group **groups, BTree_group **groupLeaderBoard, BTree_user *users, int groupid, char groupname[], int groupgoal, int Memberid[], int n)
{
    Group *Node1 = (Group *)malloc(sizeof(Group));
    Group *Node2 = (Group *)malloc(sizeof(Group));
    Node1->groupId = Node2->groupId = groupid;
    Node1->groupGoal = Node2->groupGoal = groupgoal;
    Node1->members = Node2->members = n;
    Node1->next = Node2->next = NULL;
    strcpy(Node1->groupName, groupname);
    strcpy(Node2->groupName, groupname);
    Node1->MemberDetails = Node2->MemberDetails = NULL;

    User *userNode;
    Member *newMember1, *newMember2;
    for (int i = 0; i < n; i++)
    {
        userNode = findUser(users, Memberid[i]);
        if (!userNode)
            printf("No user with UserID - %d exists\n", Memberid[i]);
        else
        {
            if (userNode->inGroup == NO)
            {
                userNode->inGroup = YES;
                newMember1 = (Member *)malloc(sizeof(Member));
                newMember2 = (Member *)malloc(sizeof(Member));
                newMember1->memberId = newMember2->memberId = Memberid[i];
                newMember1->userNode = userNode;
                newMember2->userNode = userNode;
                newMember1->next = newMember2->next = NULL;
                insertMember(&(Node1->MemberDetails), newMember1);
                insertMember(&(Node2->MemberDetails), newMember2);
            }
            else
                printf("\nAs user with UserId-%d is already inside a group cannot add him\n");
        }
    }
    if (Node1->MemberDetails == NULL)
    {
        printf("\nAs Group contains no members ");
        printf("\nGroup - %s creation failed\n", groupname);
        free(Node1);
    }
    else
    {
        *groups = InsertOnID_Group(*groups, Node1, NULL);
        *groupLeaderBoard = InsertOnSteps_Group(*groupLeaderBoard, Node2, NULL);
        printf("\nGroup - %s created Succesfully\n", groupname);
        inorder_group(*groups);
    }
}

// c)
void getTop3(BTree_user **leader, BTree_user **Users, Boolean *found, int *top, long long *leaderStepCount)
{
    if (*leader)
    {
        for (int i = 0; i < (*leader)->size; i++)
        {
            getTop3(&(*leader)->child[i], Users, found, top, leaderStepCount);
            if (*top > 0 && *found == TRUE)
            {
                printf("%d) ", 4 - *top);
                printf("Name\t\t |  %s (%d)\n", (*leader)->data[i]->name, (*leader)->data[i]->ID);
                printf("   Points \t |  %d pts\n", (*leader)->data[i]->points);
                printf("   No. of steps  |  %ld / week\n\n", stepcount((*leader)->data[i]));
                (*leaderStepCount) += stepcount((*leader)->data[i]);
                (*top)--;
            }
            else
                *found = FALSE;
        }
        getTop3(&(*leader)->child[(*leader)->size], Users, found, top, leaderStepCount);
    }
    else if (*found == FALSE)
        *found = TRUE;
}

// d)
void checkGroupAchievement(BTree_group *groups, int id)
{
    Group *currGroup = findGroup(groups, id);

    int flag = 1;
    if (!currGroup)
        printf("\nGroup-ID %d doesn't exists\n\n", id);
    else
    {
        int num = 1;
        Member *currMember = currGroup->MemberDetails;
        while (currMember)
        {
            User *currUser = currMember->userNode;
            Boolean found = FALSE;
            for (int i = 0; found == FALSE && i < 7; i++)
            {
                if (currUser->stepCount[i] < currGroup->groupGoal)
                {
                    found = TRUE;
                    flag = 0;
                    printf("\n%d) %s failed to complete the group's goal", num++, currUser->name);
                }
            }
            currMember = currMember->next;
        }
        if (flag == 1)
            printf("\n%s(ID-%d) has completed their weekly group's goal\n", currGroup->groupName, id);
        else
            printf("\n\n%s(ID-%d) failed to complete their weekly group's goal\n", currGroup->groupName, id);
        printf("\n");
    }
}

// e)
void generateLeaderBoard(BTree_group *groups, int *rank)
{
    if (groups)
    {
        for (int i = 0; i < groups->size; i++)
        {
            generateLeaderBoard(groups->child[i], rank);
            printf("\nRank          |  %d\n", (*rank)++);
            printf("No.of steps   |  %lld\n", groupStepcount(groups->data[i]));
            printGroup(groups->data[i]);
        }
        generateLeaderBoard(groups->child[groups->size], rank);
    }
}

// f)
void checkIndividualReward(BTree_user *leader, int id, Boolean *found_leader, int *Rank, int *currRank)
{
    if (leader)
    {
        for (int i = 0; i < leader->size && *Rank == 0; i++)
        {
            checkIndividualReward(leader->child[i], id, found_leader, Rank, currRank);

            if (*found_leader == TRUE && *Rank == 0)
                (*currRank)++;

            if (leader->data[i]->ID == id)
                *Rank = *currRank;
        }
        if (*Rank == 0)
            checkIndividualReward(leader->child[leader->size], id, found_leader, Rank, currRank);
    }
    else if (*found_leader == FALSE)
        *found_leader = TRUE;
}

// g)
void deleteUser(BTree_user **user, BTree_group **groups, BTree_user **leader, BTree_group **groupleader, int id)
{
    User *delNode = findUser(*user, id);
    if (delNode == NULL)
    {
        printf("No User with ID - %d \n", id);
    }
    else
    {
        if (delNode->inGroup == YES)
        {
            int flag = 0;
            deleteUserfromGroup(*groups, id, &flag);
            flag = 0;
            deleteUserfromGroup(*groupleader, id, &flag);
        }
        if (isConsistent(delNode))
        {
            *leader = DeleteSteps_USER(*leader, delNode);
        }
        printf("\nUser : %s bearing UserId - %d deleted Succesfully\n", delNode->name, delNode->ID);
        *user = DeleteID_USER(*user, id);
    }
}

// h)
void deleteGroup(BTree_group **groups, BTree_group **leader, int id)
{
    Group *group = findGroup(*groups, id);
    if (group == NULL)
    {
        printf("\n No Group with ID - %d\n", id);
    }
    else
    {
        Member *mem = group->MemberDetails;
        while (mem)
        {
            mem->userNode->inGroup = NO;
            mem = mem->next;
        }
        *leader = DeleteSteps_Group(*leader, group);
        printf("\nGroup : %s bearing GroupId - %d deleted Succesfully\n", group->groupName, group->groupId);
        *groups = DeleteID_Group(*groups, id);
    }
}

// i)
void Merge_Groups(BTree_group **groups, BTree_group **leader, int id1, int id2)
{
    Group *group1 = findGroup(*groups, id1);
    Group *group2 = findGroup(*groups, id2);

    if ((group1 == NULL) ^ (group2 == NULL))
    {
        if (group1 == NULL)
            printf("No group with ID - %d exists \n", id1);
        if (group2 == NULL)
            printf("No group with ID - %d exists \n", id2);
    }
    else
    {
        if ((group1->members + group2->members) > 5)
        {
            printf("Cannot Merge groups since both collectively contain %d members \n", (group1->members + group2->members));
        }
        else
        {
            Member *mem = group2->MemberDetails;
            while (mem)
            {
                Member *newMem = (Member *)malloc(sizeof(Member));
                newMem->memberId = mem->memberId;
                newMem->userNode = mem->userNode;
                insertMember(&(group1->MemberDetails), newMem);
                mem = mem->next;
            }
            Group *leaderId1 = NULL;
            Group *leaderId2 = NULL;
            findGroupinLeaderBoard(*leader, id1, &(leaderId1));
            findGroupinLeaderBoard(*leader, id2, &(leaderId2));
            group2->MemberDetails = NULL;
            mem = leaderId2->MemberDetails;
            while (mem)
            {
                Member *newMem = (Member *)malloc(sizeof(Member));
                newMem->memberId = mem->memberId;
                newMem->userNode = mem->userNode;
                insertMember(&(leaderId1->MemberDetails), newMem);
                mem = mem->next;
            }
            *leader = DeleteSteps_Group(*leader, leaderId2);
            leaderId2->MemberDetails = NULL;
            printf("\nGroups bearing IDs : %d ans %d are merged successfully into  group [%s] bearing ID : %d\n", group1->groupId, group2->groupId, group1->groupName, group1->groupId);
            *groups = DeleteID_Group(*groups, id2);
        }
    }
}

// j)
void displayGroupRangeInfo(BTree_group *groups, BTree_group *leaders, int lower_id, int upper_id)
{
    if (groups)
    {
        int i;
        for (i = 0; i < groups->size; i++)
        {
            if (groups->data[i]->groupId > lower_id)
                displayGroupRangeInfo(groups->child[i], leaders, lower_id, upper_id);

            if (groups->data[i]->groupId >= lower_id && groups->data[i]->groupId <= upper_id)
            {
                Boolean found = FALSE;
                int currRank = 0, rank = 0;
                findGroupRank(leaders, groups->data[i]->groupId, &found, &currRank, &rank);
                printf("\nGroup Rank    |  %d\n", rank);
                printGroup(groups->data[i]);
            }
        }

        if (groups->data[i - 1]->groupId < upper_id)
            displayGroupRangeInfo(groups->child[i], leaders, lower_id, upper_id);
    }
}

// k)
void suggestGoalUpdate(BTree_user **users, BTree_user **leaders, int id)
{
    User *targetUser = findUser(*users, id);

    if (targetUser)
    {
        int Rank = 0, currRank = 0, top = 3;
        Boolean found = FALSE;
        long long leaderStepCount = 0;
        printf("\n");
        getTop3(leaders, users, &found, &top, &leaderStepCount);
        found = FALSE;
        checkIndividualReward(*leaders, id, &found, &Rank, &currRank);

        if (Rank <= 3 && Rank != 0)
            printf("\n%s already holds rank-%d in the leaderboard!\n\n", targetUser->name, Rank);
        else
        {
            int maxsteps = targetUser->stepCount[0];
            int minsteps = targetUser->stepCount[0];
            for (int i = 1; i < 7; i++)
            {
                maxsteps = (targetUser->stepCount[i] > maxsteps) ? targetUser->stepCount[i] : maxsteps;
                minsteps = (targetUser->stepCount[i] < minsteps) ? targetUser->stepCount[i] : minsteps;
            }
            long long int leaderStepCountAvg = leaderStepCount / 21;
            if (!isConsistent(targetUser))
            {
                printf("\n%s! you are inconsistent,\nYou must aim to hit your step goal(%d steps/day) regularly.\n", targetUser->name, targetUser->dailyGoal);
                printf("\nOtherwise reduce your DailystepGoal to (%d steps/day) \n", minsteps);
                printf("\nThen continously acheive your goal daily and increment your daily goal by %d steps a week till you reach (%d steps/day)\n", maxsteps - minsteps, maxsteps);
                printf("Following that, after being consistent and you have reached (%d steps/day)\n", maxsteps);
            }
            printf("\nYou must increase your daily goal and reach around %d steps/day to rank among top 3 achievers.\n\n", leaderStepCountAvg);
        }
    }
    else
        printf("\nNo user with userID - %d exists \n", id);
}

//////////////////////// filehandling functions //////////////////////////////////////

void updateDatabase_users(BTree_user *users, FILE *file)
{
    char int_string[20];
    if (users)
    {
        for (int i = 0; i < users->size; i++)
        {
            updateDatabase_users(users->child[i], file);
            itoa(users->data[i]->ID, int_string, 10);
            fprintf(file, strcat(int_string, "\n\n"));

            fprintf(file, strcat(users->data[i]->name, "\n\n"));

            itoa(users->data[i]->age, int_string, 10);
            fprintf(file, strcat(int_string, "\n\n"));

            itoa(users->data[i]->dailyGoal, int_string, 10);
            fprintf(file, strcat(int_string, "\n\n"));

            for (int day = 0; day < 7; day++)
            {
                itoa(users->data[i]->stepCount[day], int_string, 10);
                fprintf(file, strcat(int_string, "\n"));
            }
            fprintf(file, "/\n");
        }
        updateDatabase_users(users->child[users->size], file);
    }
}

void updateDatabase_groups(BTree_group *groups, FILE *file)
{
    char int_string[20];
    if (groups)
    {
        for (int i = 0; i < groups->size; i++)
        {
            updateDatabase_groups(groups->child[i], file);
            itoa(groups->data[i]->groupId, int_string, 10);
            fprintf(file, strcat(int_string, "\n\n"));

            fprintf(file, strcat(groups->data[i]->groupName, "\n\n"));

            itoa(groups->data[i]->groupGoal, int_string, 10);
            fprintf(file, strcat(int_string, "\n\n"));

            Member *currMember = groups->data[i]->MemberDetails;
            while (currMember)
            {
                itoa(currMember->memberId, int_string, 10);
                fprintf(file, strcat(int_string, "\n"));
                currMember = currMember->next;
            }
            fprintf(file, "/\n");
        }
        updateDatabase_groups(groups->child[groups->size], file);
    }
}

void fetchDatabase(BTree_user **users, BTree_user **userLeaderBoard, BTree_group **groups, BTree_group **groupLeaderBoard)
{
    FILE *file;
    file = fopen("users.txt", "r");
    int mode = 1;
    int flag = 0;
    int i = 0;
    char line[1024];
    int id;
    char name[MAX_LEN];
    int age;
    int goal;
    int steps[7];

    if (file == NULL)
    {
        printf("Error opening file");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strlen(line) - 1] = '\0';
        if (line[0] == '/')
        {
            i = 0;
            mode = 1;

            if (flag)
            {
                User *newUser = createUser(id, name, age, goal, steps);
                if (isConsistent(newUser))
                {
                    User *temp = createUser(id, name, age, goal, steps);
                    *userLeaderBoard = InsertOnSteps_Users(*userLeaderBoard, temp, NULL);
                }
                *users = InsertOnID_Users(*users, newUser, NULL);
            }
            flag = 0;
        }
        else
        {
            if (line[0] != '\0')
            {
                switch (mode)
                {
                case 1:
                    id = atoi(line);
                    break;
                case 2:
                    strcpy(name, line);
                    break;
                case 3:
                    age = atoi(line);
                    break;
                case 4:
                    goal = atoi(line);
                    break;
                case 5:
                    steps[i] = atoi(line);
                    i++;
                    break;
                default:
                    break;
                }
            }
            else
            {
                flag = 1;
                i = 0;
                mode++;
            }
        }
    }

    fclose(file);
    file = fopen("groups.txt", "r");

    int memberIds[MAX_GROUP_STRENGTH];
    if (file == NULL)
    {
        printf("Error opening file");
        return;
    }

    flag = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strlen(line) - 1] = '\0';
        if (line[0] == '/')
        {
            if (flag)
            {
                Group *newGroup1 = createGroupNode(id, name, goal, i);
                Group *newGroup2 = createGroupNode(id, name, goal, i);
                Member *MemberNode1 = NULL, *MemberNode2 = NULL;
                for (int members = 0; members < i; members++)
                {
                    MemberNode1 = (Member *)malloc(sizeof(Member));
                    MemberNode2 = (Member *)malloc(sizeof(Member));
                    MemberNode1->next = MemberNode2->next = NULL;
                    MemberNode1->memberId = MemberNode2->memberId = memberIds[members];
                    MemberNode1->userNode = MemberNode2->userNode = findUser(*users, MemberNode1->memberId);
                    MemberNode1->userNode->inGroup = MemberNode2->userNode->inGroup = YES;
                    setGroupStatus(userLeaderBoard, memberIds[members]);
                    insertMember(&(newGroup1->MemberDetails), MemberNode1);
                    insertMember(&(newGroup2->MemberDetails), MemberNode2);
                }
                *groups = InsertOnID_Group(*groups, newGroup1, NULL);
                *groupLeaderBoard = InsertOnSteps_Group(*groupLeaderBoard, newGroup2, NULL);
            }
            mode = 1;
            i = 0;
            flag = 0;
        }
        else
        {
            if (line[0] != '\0')
            {
                switch (mode)
                {
                case 1:
                    id = atoi(line);
                    break;
                case 2:
                    strcpy(name, line);
                    break;
                case 3:
                    goal = atoi(line);
                    break;
                case 4:
                    memberIds[i] = atoi(line);
                    i++;
                    break;
                default:
                    break;
                }
            }
            else
            {
                flag = 1;
                i = 0;
                mode++;
            }
        }
    }
}

int main()
{
    system("cls");

    BTree_user *users = NULL;
    BTree_user *userLeaderBoard = NULL;
    BTree_group *groups = NULL;
    BTree_group *groupLeaderBoard = NULL;

    fetchDatabase(&users, &userLeaderBoard, &groups, &groupLeaderBoard);

    printf("WELCOME TO -- 'STEPSYNC'-- SYNC YOUR STEPS, SIMPLIFY YOUR DAY\n\n");
    printf("Do you want to perform an operation [Y/N] : ");
    char name[MAX_LEN], opstatus;
    int points = 100, top = 3;
    Boolean found = FALSE;
    scanf("%c", &opstatus);
    while (opstatus != 'y' && opstatus != 'Y' && opstatus != 'n' && opstatus != 'N')
    {
        printf("\nEnter a valid alphabet (Y/y or N/n)\n");
        printf("\nDo you want to perform an operation [Y/N] : ");
        fflush(stdin);
        scanf("%c", &opstatus);
    }
    int n, id, age, operation, count[7], dailygoal;
    while (opstatus == 'Y' || opstatus == 'y')
    {
        system("cls");
        printf("\nChoose a operation you would like to perform by entering the integer corresponding to the operation in dropdown menu \n\n");
        printf("List of operations that you can perform\n\n");
        printf(" 1  -- ADD_PERSON \n");
        printf(" 2  -- CREATE_GROUP \n");
        printf(" 3  -- GET_TOP_3 \n");
        printf(" 4  -- CHECK_GROUP_ACHIEVEMENT \n");
        printf(" 5  -- GENERATE_LEADERBOARD \n");
        printf(" 6  -- CHECK_INDIVIDUAL_REWARDS \n");
        printf(" 7  -- DELETE_INDIVIDUAL \n");
        printf(" 8  -- DELETE_GROUP \n");
        printf(" 9  -- MERGE_GROUPS \n");
        printf(" 10 -- DISPLAY_GROUP_RANGE_INFO \n");
        printf(" 11 -- SUGGEST_GOAL_UPDATE\n");
        printf("\nEnter : ");
        scanf("%d", &operation);
        while (operation < 1 || operation > 11)
        {
            printf("\nENTER A VALID NO IN RANGE FROM DROPDOWN MENU : ");
            scanf("%d", &operation);
        }
        system("cls");
        switch (operation)
        {
        case 1:
            printf("Furnish the following details to add a Individual\n");
            printf("\nEnter Name of user : ");
            fflush(stdin);
            gets(name);
            printf("\nEnter ID : ");
            scanf("%d", &id);
            printf("\nEnter Age : ");
            scanf("%d", &age);
            printf("\nEnter Daily step Goal : ");
            scanf("%d", &dailygoal);
            printf("\nEnter Daily stepcount, \n");
            for (int i = 0; i < 7; i++)
            {
                printf("\t\tDay-%d : ", i + 1);
                scanf("%d", &count[i]);
            }
            User *newUser = createUser(id, name, age, dailygoal, count);
            Add_Person(&users, &userLeaderBoard, newUser);
            printUser(newUser);
            printf("\n%s added succesfully.\n", name);
            break;
        case 2:
            printf("\nFurnish the following details to add a Group\n");
            printf("\nEnter Groupname : ");
            fflush(stdin);
            gets(name);
            printf("\nEnter ID : ");
            scanf("%d", &id);
            printf("\nEnter Weekly step Goal : ");
            scanf("%d", &dailygoal);
            printf("\nEnter no.of members in the group : ");
            scanf("%d", &n);
            while (n > 5)
            {
                printf("\nGroup strength can't exceed 5\n");
                printf("\nEnter no.of members in the group : ");
                scanf("%d", &n);
            }
            fflush(stdin);
            printf("\nEnter the Member ID's of Individuals you want to add in the Group :\n");
            for (int i = 0; i < n; i++)
            {
                printf("%d) ", i + 1);
                scanf("%d", &count[i]);
            }
            Create_group(&groups, &groupLeaderBoard, users, id, name, dailygoal, count, n);
            break;
        case 3:
            printf("\nTop 3 users ranked based on no. of steps :\n\n");
            points = 100;
            found = FALSE;
            top = 3;
            long long stepcount = 0;
            updateUserLeaderBoard(&userLeaderBoard, &users, &points, &found);
            found = FALSE;
            getTop3(&userLeaderBoard, &users, &found, &top, &stepcount);
            break;
        case 4:
            printf("\nFurnish the following details to check group achievement \n");
            printf("\nEnter GroupID : ");
            scanf("%d", &id);
            checkGroupAchievement(groups, id);
            break;
        case 5:
            printf("\nGroup LeaderBoard :\n\n");
            int rank = 1;
            generateLeaderBoard(groupLeaderBoard, &rank);
            break;
        case 6:
            printf("\nFurnish The following details to check individual rewards  \n");
            printf("\nEnter Id : ");
            scanf("%d", &id);
            points = 100;
            found = FALSE;
            int Rank = 0, currRank = 0;
            updateUserLeaderBoard(&userLeaderBoard, &users, &points, &found);
            found = FALSE;
            checkIndividualReward(userLeaderBoard, id, &found, &Rank, &currRank);
            User *targetUser = findUser(users, id);
            if (targetUser)
            {
                if (Rank != 0)
                    printf("\n%s ranks %d in the leaderboard\n", targetUser->name, Rank);
                else
                    printf("\n%s doesn't rank in the leaderboard\n", targetUser->name);
                printUser(targetUser);
            }
            else
                printf("UserID-%d not found", id);
            break;
        case 7:
            printf("\nFurnish The following details to delete an Individual  \n");
            printf("\nEnter Id : ");
            scanf("%d", &id);
            deleteUser(&users, &groups, &userLeaderBoard, &groupLeaderBoard, id);
            break;
        case 8:
            printf("\nFurnish The following details to delete a Group \n");
            printf("\nEnter Id : ");
            scanf("%d", &id);
            deleteGroup(&groups, &groupLeaderBoard, id);
            break;
        case 9:
            printf("\nFurnish The following details to merge two groups \n");
            printf("\nEnter Id of first group : ");
            scanf("%d", &id);
            printf("\nEnter Id of second group : ");
            scanf("%d", &n);
            Merge_Groups(&groups, &groupLeaderBoard, id, n);
            break;
        case 10:
            printf("\nFurnish The following details to Range-Search groups\n");
            printf("\nEnter Id of first group : ");
            scanf("%d", &id);
            printf("\nEnter Id of second group : ");
            scanf("%d", &n);
            if (id <= n)
                displayGroupRangeInfo(groups, groupLeaderBoard, id, n);
            else
                displayGroupRangeInfo(groups, groupLeaderBoard, n, id);
            break;
        case 11:
            printf("\nFurnish The following details to suggest goalUpdate \n");
            printf("\nEnter Id : ");
            scanf("%d", &id);
            points = 100;
            found = FALSE;
            updateUserLeaderBoard(&userLeaderBoard, &users, &points, &found);
            suggestGoalUpdate(&users, &userLeaderBoard, id);
            break;
        }
        printf("\n\nDo you want to perform an operation [Y/N] : ");
        fflush(stdin);
        scanf("%c", &opstatus);
        while (opstatus != 'y' && opstatus != 'Y' && opstatus != 'n' && opstatus != 'N')
        {
            printf("\nEnter a valid alphabet [(Y/y) / (N/n) ]");
            printf("\nDo you want to perform an operation [Y/N] : ");
            fflush(stdin);
            scanf("%c", &opstatus);
        }
    }
    system("cls");

    FILE *file = fopen("new_users.txt", "w");
    fprintf(file, "/\n");
    updateDatabase_users(users, file);
    fclose(file);
    file = fopen("new_groups.txt", "w");
    fprintf(file, "/\n");
    updateDatabase_groups(groups, file);
    fclose(file);
    printf("\n\n\n\n\n\n\n\n\n\n\n\nTHANKS FOR CHOOSING -'STEPSYNC'- HAVE A HEALTHY AND FIT DAY\n\n\n\n\n\n\n\n\n\n\n\n");

    return 0;
}