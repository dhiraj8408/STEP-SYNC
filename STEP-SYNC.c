#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 50
#define MAX_GROUP_STRENGTH 5

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
    Boolean dailyGoalStatus[7];
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

// helper functions start
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
    for (int i = 0; i < 7; i++)
    {
        if (step[i] >= goal)
            create->dailyGoalStatus[i] = TRUE;
        else
            create->dailyGoalStatus[i] = FALSE;
    }
}

User *findUser(User *head, int Id)
{
    User *ret_val = NULL;
    User *ptr = head;
    Boolean found = FALSE;
    while (ptr && !found)
    {
        if (ptr->ID == Id)
        {
            found = TRUE;
            ret_val = ptr;
        }
        ptr = ptr->next;
    }
    return ret_val;
}

User *InsertUser(User *head, User *newUser)
{
    if (head == NULL || head->ID > newUser->ID)
    {
        newUser->next = head;
        head = newUser;
    }
    else
    {
        User *curr = head;
        User *prev = NULL;
        while (curr && curr->ID < newUser->ID)
        {
            prev = curr;
            curr = curr->next;
        }
        newUser->next = curr;
        prev->next = newUser;
    }
    return head;
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

Group *InsertGroup(Group *groups, Group *newGroup)
{
    if (groups == NULL || groups->groupId > newGroup->groupId)
    {
        newGroup->next = groups;
        groups = newGroup;
    }
    else
    {
        Group *curr = groups;
        Group *prev = NULL;
        while (curr && curr->groupId < newGroup->groupId)
        {
            prev = curr;
            curr = curr->next;
        }
        newGroup->next = curr;
        prev->next = newGroup;
    }
    return groups;
}

void printUser(User *head)
{
    printf("\n\n");
    printf("Name         | %s\n", head->name);
    printf("ID           | %d\n", head->ID);
    printf("Age          | %d\n", head->age);
    printf("Daily Goal   | %d steps\n", head->dailyGoal);

    if (head->inGroup)
        printf("In group     | YES\n");
    else
        printf("In group     | NO\n");

    printf("\nWeekly step count analysis: \n");
    for (int i = 0; i < 7; i++)
    {
        if (head->dailyGoalStatus[i])
            printf("Day - %d | Step count : %d | Daily Goal Status : Done\n", i + 1, head->stepCount[i]);
        else
            printf("Day - %d | Step count : %d | Daily Goal Status : Not Done\n", i + 1, head->stepCount[i]);
    }
    printf("\n");
}

void displayUsers(User *head)
{
    User *ptr = head;
    while (ptr)
    {
        printUser(ptr);
        ptr = ptr->next;
    }
}

void printGroup(Group *currGroup)
{
    int i = 1;
    printf("\nGroup ID      |  %d\n", currGroup->groupId);
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

void displayGroups(Group *groups)
{
    Group *currGroup = groups;
    printf("\n");
    while (currGroup)
    {
        printGroup(currGroup);
        currGroup = currGroup->next;
        printf("\n");
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
        isconsistent = (isconsistent && (head->dailyGoalStatus[i]));
        i++;
    }
    return isconsistent;
}

void InsertUserLeaderboard(User **leader, User *newuser)
{
    Boolean isCons = isConsistent(newuser);
    if (isCons)
    {
        long long int steps = stepcount(newuser);
        if (*leader == NULL)
            *leader = newuser;
        else
        {
            User *curr = *leader;
            User *prev = NULL;
            while (curr && stepcount(curr) > steps)
            {
                prev = curr;
                curr = curr->next;
            }

            if (curr && stepcount(curr) == steps)
            {
                if (newuser->ID > prev->ID)
                {
                    prev = curr;
                    curr = curr->next;
                }
            }

            newuser->next = curr;
            if (prev)
                prev->next = newuser;
            else
            {
                newuser->next = curr;
                *leader = newuser;
            }
        }
    }
}

long long int groupStepCount(Group *node)
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

Group *Divide(Group *head)
{
    Group *slow = head;
    Group *fast = head;
    Group *prev = NULL;
    while (fast && fast->next)
    {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    prev->next = NULL;
    return slow;
}

Group *Mergeonsteps(Group *head1, Group *head2)
{
    Group *ptr1 = head1;
    Group *ptr2 = head2;
    Group *mergedhead = NULL;
    if (groupStepCount(ptr1) > groupStepCount(ptr2))
    {
        mergedhead = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        mergedhead = ptr2;
        ptr2 = ptr2->next;
    }
    Group *ptr = mergedhead;
    while (ptr1 && ptr2)
    {
        if (groupStepCount(ptr1) > groupStepCount(ptr2))
        {
            ptr->next = ptr1;
            ptr = ptr->next;
            ptr1 = ptr1->next;
        }
        else
        {
            ptr->next = ptr2;
            ptr = ptr->next;
            ptr2 = ptr2->next;
        }
    }
    if(ptr1)
        ptr->next = ptr1;
    else if(ptr2)
        ptr->next = ptr2;
    
    return mergedhead;
}

Group *SortOnsteps(Group *head)
{
    if (head && head->next)
    {
        Group *mid = Divide(head);
        Group *head1 = SortOnsteps(head);
        Group *head2 = SortOnsteps(mid);
        head = Mergeonsteps(head1, head2);
    }
    return head;
}

Group *MergeonID(Group *head1, Group *head2)
{
    Group *ptr1 = head1;
    Group *ptr2 = head2;
    Group *mergedhead = NULL;
    if (ptr1->groupId < ptr2->groupId)
    {
        mergedhead = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        mergedhead = ptr2;
        ptr2 = ptr2->next;
    }
    Group *ptr = mergedhead;
    while (ptr1 && ptr2)
    {
        if (ptr1->groupId < ptr2->groupId)
        {
            ptr->next = ptr1;
            ptr = ptr->next;
            ptr1 = ptr1->next;
        }
        else
        {
            ptr->next = ptr2;
            ptr = ptr->next;
            ptr2 = ptr2->next;
        }   
    }
    if(ptr1)
        ptr->next = ptr1;
    else if(ptr2)
        ptr->next = ptr2;
    
    return mergedhead;
}

Group *SortOnID(Group *head)
{
    if (head && head->next)
    {
        Group *mid = Divide(head);
        Group *head1 = SortOnID(head);
        Group *head2 = SortOnID(mid);
        head = MergeonID(head1, head2);
    }
    return head;
}

void updateUserLeaderBoard(User **Users,User **leader)
{
    User *curr = *leader;
    int count = 3;
    int points = 100;
    while(count > 0 && curr){
        User* usernode = findUser(*Users,curr->ID);
        if(usernode)
            usernode->points += points;
        curr->points += points;
        points -= 25;
        count--;
        curr = curr->next;
    }
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

// helper functions end

// a)
void Add_Person(User **head, User **leader, User *Node)
{
    if (*head == NULL || (*head)->ID > Node->ID)
    {
        Node->next = *head;
        *head = Node;
    }
    else
    {
        User *curr = *head;
        User *prev = NULL;
        while (curr && curr->ID < Node->ID)
        {
            prev = curr;
            curr = curr->next;
        }
        Node->next = curr;
        prev->next = Node;
    }
    if (isConsistent(Node))
    {
        User *temp = createUser(Node->ID, Node->name, Node->age, Node->dailyGoal, Node->stepCount);
        InsertUserLeaderboard(leader, temp);
    }
}

// b)
void Create_group(Group **head, int groupid, char groupname[], int groupgoal, int Memberid[], int n, User *Users)
{
    Group *Node = (Group *)malloc(sizeof(Group));
    Node->groupId = groupid;
    Node->groupGoal = groupgoal;
    Node->members = n;
    Node->next = NULL;
    strcpy(Node->groupName, groupname);
    Node->MemberDetails = NULL;
    User *userNode;
    Member *newMember;
    for (int i = 0; i < n; i++)
    {
        userNode = findUser(Users, Memberid[i]);
        if (!userNode)
            printf("No user with UserID - %d exists\n", Memberid[i]);
        else
        {
            if (userNode->inGroup == NO)
            {
                userNode->inGroup = YES;
                newMember = (Member *)malloc(sizeof(Member));
                newMember->memberId = Memberid[i];
                newMember->userNode = userNode;
                newMember->next = NULL;
                insertMember(&(Node->MemberDetails), newMember);
            }
            else
                printf("\nAs user with UserId-%d is already inside a group cannot add him\n");
        }
    }
    if (Node->MemberDetails == NULL)
    {
        printf("\nAs Group contains no members ");
        printf("\nGroup - %s creation failed\n", groupname);
        free(Node);
    }
    else
    {
        *head = InsertGroup(*head, Node);
        printf("\nGroup - %s created Succesfully ", groupname);
        displayGroups(*head);
    }
}

// c)
void Get_top_3(User **leader)
{
    int count = 1;
    User *ptr = *leader;
    while (ptr && count <= 3)
    {
        printf("%d) ", count++);
        printf("Name\t\t |  %s (%d)\n", ptr->name, ptr->ID);
        printf("   No. of steps  |  %ld / week\n\n", stepcount(ptr));
        ptr = ptr->next;
    }
}

// d)
void checkGroupAchievement(Group *groups, int id)
{
    Group *currGroup = groups;
    Boolean found = FALSE;
    while (currGroup && found == FALSE)
    {
        if (currGroup->groupId == id)
            found = TRUE;
        else
            currGroup = currGroup->next;
    }

    int flag = 1;
    if (!found)
        printf("\nGroup-ID %d doesn't exists\n\n", id);
    else
    {
        int num = 1;
        Member *currMember = currGroup->MemberDetails;
        while (currMember)
        {
            User *currUser = currMember->userNode;
            found = FALSE;
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
void Generate_Leader_board(Group **head)
{
    *head = SortOnsteps(*head);
    Group *ptr = *head;
    int count = 1;
    while (ptr)
    {
        printf("\n");
        printf("Rank          | %d\n", count++);
        printf("No.of steps   | %lld", groupStepCount(ptr));
        printGroup(ptr);
        ptr = ptr->next;
    }
    *head = SortOnID(*head);
}

// f)
void checkIndividualReward(User* Users,User *leaderBoard, int id)
{
    updateUserLeaderBoard(&Users,&leaderBoard);
    int top = 3;
    Boolean found = FALSE;
    User *currLeader = leaderBoard;
    while (currLeader && found == FALSE)
    {
        if (currLeader->ID == id)
            found = TRUE;
        else{
            currLeader = currLeader->next;
            top--;
        }
    }
    User *Usernode = findUser(Users,id);

    if(Usernode == NULL)
        found = FALSE;
    else
        found = TRUE;
    
    if (!found)
        printf("\nUserID-%d doesn't exists\n", id);
    else
    {
        printUser(Usernode);
        if(top > 0){
            printf("\n%s ranks among top 3", Usernode->name);
            printf("\nRank   : %d",4 - top);
        }
        printf("\nReward :  %d pts\n\n", Usernode->points);
    }
}

// g)
void deleteUser(User **Users, Group **groups, User **leader, int id)
{
    User *prevUser = NULL;
    User *currUser = *Users;
    Boolean found = FALSE;

    while (currUser && found == FALSE)
    {
        if (currUser->ID == id)
            found = TRUE;
        else
        {
            prevUser = currUser;
            currUser = currUser->next;
        }
    }

    if (found)
    {
        User *ptr = *leader;
        User *prevptr = NULL;
        Boolean Foundinleader = FALSE;
        while (!Foundinleader && ptr)
        {
            if (ptr->ID == id)
                Foundinleader = TRUE;
            else
            {
                prevptr = ptr;
                ptr = ptr->next;
            }
        }
        if (Foundinleader)
        {
            if (prevptr)
                prevptr->next = ptr->next;
            else
                *leader = ptr->next;
            ptr->next = NULL;
            free(ptr);
        }
        if (currUser->inGroup == YES)
        {
            found = FALSE;
            Group *prevGroup = NULL;
            Group *currGroup = *groups;
            while (currGroup && found == FALSE)
            {
                Member *prevMember = NULL;
                Member *currMember = currGroup->MemberDetails;
                while (currMember && found == FALSE)
                {
                    if (currMember->memberId == id)
                        found = TRUE;
                    else
                    {
                        prevMember = currMember;
                        currMember = currMember->next;
                    }
                }
                if (found)
                {
                    if (prevMember)
                        prevMember->next = currMember->next;
                    else
                        currGroup->MemberDetails = currGroup->MemberDetails->next;
                    free(currMember);
                }
                else{
                    prevGroup = currGroup;
                    currGroup = currGroup->next;
                }
            }
            if(found){
                if(currGroup->MemberDetails == NULL){
                    if(prevGroup)
                        prevGroup->next = currGroup->next;
                    else
                        *groups = currGroup->next;
                    printf("\nSince %s - is the only user present in group - %s  is also deleted\n",currUser->name,currGroup->groupName);
                    free(currGroup);
                }
            }
        }
        if (prevUser)
            prevUser->next = currUser->next;
        else
            *(Users) = currUser->next;
        printf("\n%s deleted successfully\n", currUser->name);
        free(currUser);
    }
    else
        printf("\nUser-ID %d doesn't exists\n", id);
}

// h)
void deleteGroup(Group **groups, int id, int flag)
{
    Group *prevGroup = NULL;
    Group *currGroup = *groups;
    Boolean found = FALSE;

    while (currGroup && found == FALSE)
    {
        if (currGroup->groupId == id)
            found = TRUE;
        else
        {
            prevGroup = currGroup;
            currGroup = currGroup->next;
        }
    }

    if (found)
    {
        Member *memberList = currGroup->MemberDetails;
        while (memberList)
        {
            if (flag)
                memberList->userNode->inGroup = NO;
            memberList = memberList->next;
        }
        if (prevGroup)
            prevGroup->next = currGroup->next;
        else
            *groups = currGroup->next;
        if (flag)
            printf("\nGroup %s deleted succesfully\n", currGroup->groupName);
        free(currGroup);
    }
    else
        printf("\nGroup-ID %d doesn't exists\n", id);
}

// i)
void MergeGroups(Group **groups, int id1, int id2)
{
    Group *group1 = NULL, *group2 = NULL;
    Group *currGroup = *groups;
    int groupFounds = 0;
    while (currGroup && groupFounds < 2)
    {
        if (currGroup->groupId == id1)
        {
            group1 = currGroup;
            groupFounds++;
        }
        if (currGroup->groupId == id2)
        {
            group2 = currGroup;
            groupFounds++;
        }
        currGroup = currGroup->next;
    }

    if (group1 == NULL)
        printf("\nGroup-ID %d doesn't exists\n", id1);
    else if (group2 == NULL)
        printf("\nGroup-ID %d doesn't exists\n", id2);
    else if (group1->members + group2->members > 5)
        printf("\nGroups can't be merged, as it will exceed group strength of 5\n");
    else
    {
        char newGroupName[MAX_LEN];
        int newGroupGoal, newGroupId;
        Group *newGroup = (Group *)malloc(sizeof(Group));

        fflush(stdin);
        printf("\nEnter new group name: ");
        gets(newGroupName);
        strcpy(newGroup->groupName, newGroupName);

        printf("Set %s's goals: ", newGroupName);
        scanf("%d", &newGroupGoal);
        newGroup->groupGoal = newGroupGoal;

        printf("Set %s's ID: ", newGroupName);
        scanf("%d", &newGroupId);
        newGroup->groupId = newGroupId;

        newGroup->members = group1->members + group2->members;

        Member *memberList = NULL, *tail = NULL;
        Member *member1 = createMemberList(group1->MemberDetails);
        Member *member2 = createMemberList(group2->MemberDetails);

        if (member1->memberId < member2->memberId)
        {
            memberList = tail = member1;
            member1 = member1->next;
        }
        else
        {
            memberList = tail = member2;
            member2 = member2->next;
        }

        while (member1 && member2)
        {
            if (member1->memberId < member2->memberId)
            {
                tail->next = member1;
                tail = member1;
                member1 = member1->next;
            }
            else
            {
                tail->next = member2;
                tail = member2;
                member2 = member2->next;
            }
        }

        if (member1)
            tail->next = member1;
        if (member2)
            tail->next = member2;

        newGroup->MemberDetails = memberList;
        newGroup->next = NULL;
        deleteGroup(groups, id1, 0);
        deleteGroup(groups, id2, 0);
        *groups = InsertGroup(*groups, newGroup);
    }
}

// j)
void display_group_info(Group **head)
{
    *head = SortOnsteps(*head);
    Group *ptr = *head;
    int rank = 1;
    while (ptr)
    {
        printf("\nRank : %d ", rank++);
        printGroup(ptr);
        ptr = ptr->next;
    }
    *head = SortOnID(*head);
}

// k)
void suggestGoalUpdate(User *users, User *leaders, int id)
{
    User *currLeader = leaders;
    User *person = findUser(users, id);
    if (person)
    {
        long long int leaderStepCount = 0;
        int flag = 0;
        int top = 3;
        while (top-- && !flag)
        {
            if (currLeader->ID == id)
            {
                printf("\n%s already holds rank-%d in the leaderboard!\n\n", currLeader->name, 3 - top);
                flag = 1;
            }
            else
            {
                leaderStepCount += stepcount(currLeader);
                currLeader = currLeader->next;
            }
        }

        if (!flag)
        {
            int maxsteps = person->stepCount[0];
            int minsteps = person->stepCount[0];
            for (int i = 1; i < 7; i++)
            {
                maxsteps = (person->stepCount[i] > maxsteps) ? person->stepCount[i] : maxsteps;
                minsteps = (person->stepCount[i] < minsteps) ? person->stepCount[i] : minsteps;
            }
            long long int leaderStepCountAvg = leaderStepCount / 21;
            if (!isConsistent(person))
            {
                printf("\n%s You are inconsistent,\nYou must aim to hit your step goal(%d steps/day) regularly.\n", person->name, person->dailyGoal);
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

// filehandling functions
void updateDatabase(User *users, Group *groups)
{
    FILE *file;
    file = fopen("new_database.txt", "w");
    User *currUser = users;
    char int_string[20];

    fprintf(file, "/\n");
    while (currUser)
    {
        itoa(currUser->ID, int_string, 10);
        fprintf(file, strcat(int_string, "\n\n"));

        fprintf(file, strcat(currUser->name, "\n\n"));

        itoa(currUser->age, int_string, 10);
        fprintf(file, strcat(int_string, "\n\n"));

        itoa(currUser->dailyGoal, int_string, 10);
        fprintf(file, strcat(int_string, "\n\n"));

        for (int i = 0; i < 7; i++)
        {
            itoa(currUser->stepCount[i], int_string, 10);
            fprintf(file, strcat(int_string, "\n"));
        }
        fprintf(file, "/\n");
        currUser = currUser->next;
    }
    fclose(file);

    file = fopen("new_group.txt", "w");
    Group *currGroup = groups;

    fprintf(file, "/\n");
    while (currGroup)
    {
        itoa(currGroup->groupId, int_string, 10);
        fprintf(file, strcat(int_string, "\n\n"));

        fprintf(file, strcat(currGroup->groupName, "\n\n"));

        itoa(currGroup->groupGoal, int_string, 10);
        fprintf(file, strcat(int_string, "\n\n"));

        Member *currMember = currGroup->MemberDetails;
        while (currMember)
        {
            itoa(currMember->memberId, int_string, 10);
            fprintf(file, strcat(int_string, "\n"));
            currMember = currMember->next;
        }
        fprintf(file, "/\n");
        currGroup = currGroup->next;
    }
    fclose(file);
    printf("\n\n\n\n\n\nTHANKS FOR CHOOSING -'STEPSYNC'- HAVE A HEALTHY AND FIT DAY\n\n\n\n\n\n\n\n\n\n\n\n");
}

void fetchDatabase(User **users, User **userleaderBoard, Group **groups)
{
    FILE *file;
    file = fopen("database.txt", "r");
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
                    InsertUserLeaderboard(userleaderBoard, temp);
                }
                *users = InsertUser(*users, newUser);
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
                Group *newGroup = createGroupNode(id, name, goal, i);
                Member *MemberNode = NULL;
                for (int members = 0; members < i; members++)
                {
                    MemberNode = (Member *)malloc(sizeof(Member));
                    MemberNode->next = NULL;
                    MemberNode->memberId = memberIds[members];
                    MemberNode->userNode = findUser(*users, MemberNode->memberId);
                    MemberNode->userNode->inGroup = YES;
                    insertMember(&(newGroup->MemberDetails), MemberNode);
                }
                *groups = InsertGroup(*groups, newGroup);
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

    User *users = NULL;
    User *userleaderBoard = NULL;
    Group *groups = NULL;

    fetchDatabase(&users, &userleaderBoard, &groups);

    printf("WELCOME TO -- 'STEPSYNC'-- SYNC YOUR STEPS, SIMPLIFY YOUR DAY\n\n");
    printf("Do you want to perform an operation [Y/N] : ");
    char name[MAX_LEN], opstatus;
    scanf("%c", &opstatus);
    while (opstatus != 'y' && opstatus != 'Y' && opstatus != 'n' && opstatus != 'N')
    {
        printf("\nEnter a valid alphabet [(Y/y) / (N/n) ]");
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
        printf(" 10 -- DISPLAY_GROUP_INFO \n");
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
            Add_Person(&users, &userleaderBoard, newUser);
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
            Create_group(&groups, id, name, dailygoal, count, n, users);
            break;
        case 3:
            printf("\nTop 3 users ranked based on no. of steps :\n\n");
            Get_top_3(&userleaderBoard);
            break;
        case 4:
            printf("\nFurnish the following details to check group achievement \n");
            printf("\nEnter GroupID : ");
            scanf("%d", &id);
            checkGroupAchievement(groups, id);
            break;
        case 5:
            printf("\nGroup LeaderBoard :\n\n");
            Generate_Leader_board(&groups);
            break;
        case 6:
            printf("\nFurnish The following details to check individual rewards  \n");
            printf("\nEnter Id : ");
            scanf("%d", &id);
            checkIndividualReward(users,userleaderBoard, id);
            break;
        case 7:
            printf("\nFurnish The following details to delete an Individual  \n");
            printf("\nEnter Id : ");
            scanf("%d", &id);
            deleteUser(&users, &groups, &userleaderBoard, id);
            break;
        case 8:
            printf("\nFurnish The following details to delete a Group \n");
            printf("\nEnter Id : ");
            scanf("%d", &id);
            deleteGroup(&groups, id, 1);
            displayGroups(groups);
            break;
        case 9:
            printf("\nFurnish The following details to delete a Group \n");
            printf("\nEnter Id of first group : ");
            scanf("%d", &id);
            printf("\nEnter Id of second group : ");
            scanf("%d", &n);
            MergeGroups(&groups, id, n);
            displayGroups(groups);
            break;
        case 10:
            printf("\nGroup info: \n");
            display_group_info(&groups);
            break;
        case 11:
            printf("\nFurnish The following details to suggest goalUpdate \n");
            printf("\nEnter Id : ");
            scanf("%d", &id);
            suggestGoalUpdate(users, userleaderBoard, id);
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

    updateDatabase(users, groups);

    return 0;
}