# STEP-SYNC

**Description:**  
This Step Tracking Application is designed to help individuals track their daily steps, set weekly goals, create groups to achieve collective goals, and earn rewards based on their achievements. The application maintains leaderboards for both individuals and groups to encourage competition and motivation. This is built in C with the use of Singly Linked list.

**Data Structures:** 

- **Individuals Linked List:** A linked list to store information about individuals.
  - Fields:
    - ID (unique identifier)
    - Name
    - Age
    - Daily Step Goal
    - Array of Weekly Step Counts (7 days)
    - Points (or) Rewards
    - Group status (YES/NO)
    - Boolean array which stores daily goal completion status
  - Sorting: Based on ID

- **Member Linked List:** A linked list which stores the ID of an individual and a pointer to the user node which is a part of the group.
  - Fields:
    - MemberId
    - Pointer to user node (User *ptr)
  - Sorting: Based on Member Id
- **Groups Linked List:** A linked list to store information about groups.
  - Fields:
    - Group-ID
    - Group Name
    - No. of members
    - Linked list of type Member with size of at most 5
    - Weekly Group Goal
  - Sorting: Based on Group-ID

**Functionality:**
- Add_Person(ID, Name, Age, Daily Step Goal): Add a new individual to the list of individuals. Maintain sorting based on ID.
- Create_group(Group-ID, Group Name, Member IDs): Create a new group and add existing individuals to it from the individual list. Individuals already belonging to a group cannot be added to a new group.
- Get_top_3(): Display the top 3 individuals who have completed their daily step goals and achieved the highest number of steps.
- Check_group_achievement(Group-ID): Display whether the given group has completed its weekly group goal.
- Generate_leader_board(): Display a leaderboard of groups sorted in descending order of steps completed.
- Check_individual_rewards(ID): Display the rewards earned by an individual if they are in the top 3 individuals.
- Delete_individual(ID): Delete an individual from the list of individuals and remove them from their groups.
- Delete_group(Group-ID): Delete a group but retain its individuals in the individual list.
- Merge_groups(Group_ID_1, Group_ID_2): Create a new group by merging two groups and set new goals.
- Display_group_info(): Display information about members in the group as well as group goals and rank.
- Suggest_goal_update(): Suggest a daily goal update for an individual to consistently appear in the top 3 individuals.

**Implementation Details:**
- The application is implemented in C programming language.
- Data structures used: Singly linked lists for individuals and groups.
- File handling is used for data input and output.
- The user interface is command based example images of user interface are attached to the repository
