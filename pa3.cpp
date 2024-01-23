// === Region: Project Overview ===
//
//  COMP2011 Fall 2023
//  PA3: A Simplified Version of USTSPAC
//
//  Your name: LOH, Angus Han Jern
//  Your ITSC email: ahjloh@connect.ust.hk
//
//  Project TA: PAPPAS Christodoulos (cpappas@connect.ust.hk); 
//              XU Shuangjie (shuangjie.xu@connect.ust.hk)
//
//  For code-level questions, please send a direct email to the above TA.
//  Asking questions with code  in a public discussion forum (e.g., Piazza) may
//  cause plagiarism issues Usually, you will get the quickest response via a
//  direct email.
//
// =====================================

// === Region: Header files ===
// Necessary header files are included,
// DO NOT include extra header files
// ============================
#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;

const int MAX_RANKING_STARS =
    5; // at most a 5-star ranking in coments (from 1-star)
const int MAX_TITLE =
    100; // at most 100 characters (including the NULL character)

// A sorted linked list of StockItem, sorted by its id
struct Student {
  unsigned int sid;     // id is an unique identifier of the Student (e.g., 39)
  char name[MAX_TITLE]; // title is a description of the Course (e.g., History)
  unsigned int ranks_count; // The total number of star_ranks on existing
                               // courses the student did until now
  Student *next;               // The pointer pointing to the next Student
};

struct StarRank {
  unsigned int star; // The star-ranking the student gave to that course
  Student *student;  // The pointer showing to the struct of the student that
                     // made the star_rank
  StarRank *next;     // The pointer pointing to the StarRanks struct
};

// A sorted linked list represents a shopping cart, sorted by item->id
struct Course {
  unsigned int course_id; // course_id is an unique identifier of the Course
                          // (e.g., History)
  char name[MAX_TITLE];   // The course name
  int stars_count[MAX_RANKING_STARS]; // The count of stars from 1 (lowest) to
                                      // MAX_RANK (highest rank) of the course
  StarRank *star_rank_head; // The pointer pointing to the StarRanks struct
};

Student *create_student(const unsigned int sid, const char name[MAX_TITLE]) {
  Student *new_student = new Student;
  new_student->sid = sid;
  strcpy(new_student->name, name);
  new_student->ranks_count = 0;
  new_student->next = nullptr;
  return new_student;
}

Course *create_course(const unsigned int course_id,
                      const char name[MAX_TITLE]) {
  Course *new_course = new Course;
  new_course->course_id = course_id;
  strcpy(new_course->name, name);
  for (int i = 0; i < MAX_RANKING_STARS; i++) {
    new_course->stars_count[i] = 0;
  }
  new_course->star_rank_head = nullptr;
  return new_course;
}

// Given the number of courses, dynamicially creates and initializes the courses
// list array
Course **dynamic_init_course_array(const unsigned int num_courses) {
  Course **ret = nullptr;
  ret = new Course *[num_courses];
  for (int i = 0; i < num_courses; i++)
    ret[i] = nullptr;
  return ret;
}

// Helper function: search student and return prev, current
// return true if found an existing entry
// return false if an existing entry is not found
bool search_student(Student *head, const unsigned int sid, Student *&prev,
                    Student *&current) {
  prev = nullptr;
  for (current = head; current != nullptr; current = current->next) {
    if (current->sid == sid) {
      // found an existing entry
      return true;
    } else if (current->sid > sid) {
      // cannot find an existing entry
      return false;
    }
    prev = current;
  }
  return false;
}

// Helper function: search star_rank and return prev, current
// return true if found an existing entry
// return false if an existing entry is not found
bool search_star_rank(StarRank *head, const unsigned int sid, StarRank *&prev,
                    StarRank *&current) {
  prev = nullptr;
  for (current = head; current != nullptr; current = current->next) {
    if (current->student->sid == sid) {
      // found an existing entry
      return true;
    }
    prev = current;
  }
  return false;
}

// Helper function: search course and return prev, current
// return true if found an existing entry
// return false if an existing entry is not found
bool search_course(Course **&course_array, const unsigned int course_id,
                   const unsigned int num_courses, int &i) {
  Course *course;
  if (course_array != nullptr) {
    for (i = 0; i < num_courses; i++) {
      course = course_array[i];
      if (course == nullptr) {
        break;
      }

      if (course->course_id == course_id) {
        // found an existing entry
        return true;
      }
    }
  }
  return false;
}

// Adds a new course in the courses array of pointers.
// If the course exists (there is a course with the course_id), return false.
// Else if the course does not exist and the array has empty space then insert the course and return true.
// Else if there is no empty space, double the array size (e.g., if the array has size 16, then increase it to size 32), and then add the course. Finally return true.
// The items of stars_count array of a newly added course must be all zero. Moreover, the star_rank_head field should be nullptr because there are no star ranks yet.

// @param: course_array the array of pointers for the Course
// @param: an unsigned integer representing the id of the course the student star ranks (course_id)
// @param: a characters array for the name of the course
// @param: an unsigned integer representing the number of courses until now.
bool add_course(Course **&course_array, const unsigned int course_id,
                const char name[MAX_TITLE], unsigned int &num_courses) {
  // TODO: Write code to implement add_course
  Course* new_course_p = create_course(course_id, name);
  int count = 0;
  bool ret = search_course(course_array, course_id, num_courses, count);

  if (ret == false){ 
    if (count == num_courses){
      num_courses*=2;
      cout << "increase course array size to " << num_courses << endl;

      Course** new_array = dynamic_init_course_array(num_courses);
      for (int i = 0; i< num_courses/2; i++){
        new_array[i] = course_array[i];
      }
      delete [] course_array;
      course_array = nullptr;
      course_array = dynamic_init_course_array(num_courses);
      for (int i = 0; i< num_courses; i++){
        course_array[i] = new_array[i];
      }
      delete [] new_array;
      new_array = nullptr;
  }
    course_array[count] = new_course_p;
    return true;
  }
  delete new_course_p;
  new_course_p = nullptr;
  return false;
}

//Adds the star ranking of a student for a course.
//If the student or the course does not exist, return false.
//Else if the student already has a star ranking for that course then return return false.
//Else, add a star ranking, update the stars_count of the course, and increase by one the ranks_count of the student and return true.

// IMPORTANT: Always add in a new StarRank at the end of the list

// @param: student_head points to the head of Student list
// @param: the id of the student (sid)
// @param: course_array the array of pointers for the Course
// @param: the id of the course the student ranks (course_id)
// @param: the number of courses in the site
// @param: the rating which is a between 1 and MAX_RANKING_STARS
// @out: a boolean value indicating whether the insertion was successful
bool add_star_rank(Student *&student_head, unsigned int sid,
                 Course **&course_array, unsigned int course_id,
                 const unsigned int num_courses, int star) {
  // TODO: Write code to implement add_star_rank
  // use error cout carefully
  Student* current = nullptr;
  Student* previous = nullptr;
  bool ret = search_student(student_head, sid, previous, current);
  if (ret == false){
    cout << "Failed to find student " << sid << " when add a star_rank." << endl;
    return false;
  }
  else{
    int count = 0;
    bool ret_course = search_course(course_array, course_id, num_courses, count);
    if (ret_course == true){
      StarRank* curr = nullptr;
      StarRank* prev = nullptr;
      bool ret_star_rank = search_star_rank(course_array[count]->star_rank_head, sid, prev, curr);
      if (ret_star_rank == true){
        cout << "Failed to insert star_rank because the student " << sid
            << " already have a star_rank." << endl;
        return false;
      }
      else{
        StarRank* new_star_rank = new StarRank;
        if (course_array[count]->star_rank_head == nullptr){
          course_array[count]->star_rank_head = new_star_rank;
        }
        else{
          for(StarRank* p = course_array[count]->star_rank_head; p != nullptr; p=p->next){
            if (p->next == nullptr){
              p->next = new_star_rank;
              break;
            }
          }
        }
        new_star_rank->star = star;
        new_star_rank->student = current;
        new_star_rank->next = nullptr;
        current->ranks_count +=1;
        course_array[count]->stars_count[star-1] += 1;
        return true;
        //delete new_star_rank;
        new_star_rank = nullptr;
      }
    }
    else{
      cout << "Failed to find course " << course_id << " when add a star_rank." << endl;
      return false;
    }

  }


  return false;
}

//Adds a student to the student's linked list.
//Takes as input the student id and its name and adds the student to the list, in increasing order based on its student id.
//Note that if the student already exists (there is a student with the same sid), then return false.
//Otherwise, add the student (maintaining the increasing order) and return true. A new student always has ranks_count = 0.
bool add_student(Student *&student_head, const unsigned int sid,
                 const char name[MAX_TITLE]) {
  // TODO: Write code to implement add_student


  Student* new_student_p = create_student(sid, name);
  if (student_head == nullptr){

    student_head = new_student_p;
    return true;
  }
  

  else if (new_student_p->sid < student_head->sid){
    new_student_p->next = student_head;
    student_head = new_student_p;
    return true;
    //new_student_p->next = nullptr;
  }

  else{
    Student* current = nullptr;
    Student* prev = nullptr;
    bool ret = search_student(student_head, sid, prev, current);
    if (ret == true){
      delete new_student_p;
      return false;
    }

    else if (ret == false){
      for (Student* p = student_head; p != nullptr; p=p->next ){
        if (p->next == nullptr){
          p->next = new_student_p;
          return true;
          break;
        }
        
        //if ((new_student_p->sid > p->sid) && (new_student_p->sid < p->next->sid)){
        if (new_student_p->sid < p->next->sid){
          new_student_p->next = p->next;
          p->next = new_student_p;
          return true;
          break;
        }
        
      }
    }
    current = nullptr;
    prev = nullptr;

  }

  delete new_student_p;
  new_student_p = nullptr;
  return false;
}

//Removes the star ranking of a student for a course.
//If the star ranking does not exist, return false.
//Else, delete the star ranking, decrease the ranks_count of the student with id sid, and
// update the new stars_count of the course with id course_id. Finally return true.

// @param: student_head points to the head of Student list
// @param: course_array the array of pointers for the Course
// @param: the id of the student (sid)
// @param: the id of the course the student ranks (course_id)
// @param: the number of courses in the site
// @out: a boolean value indicating whether the removal was successful
bool delete_star_rank(Student *&student_head, Course **&course_array,
                    const unsigned int sid, const unsigned int course_id,
                    const unsigned int num_courses) {
  // TODO: Write code to implement delete_star_rank
  // use error cout carefully
  int count = 0;
  bool ret_course = search_course(course_array, course_id, num_courses, count);
  if (ret_course == false){
    cout << "Failed to delete star_rank, course " << course_id << " not found."
        << endl;
    return false;
  }
  else{
    StarRank* prev = nullptr;
    StarRank* current = nullptr;
    bool ret_star_rank = search_star_rank(course_array[count]->star_rank_head, sid, prev, current);
    if (ret_star_rank == true){
      StarRank* previous = nullptr;
      for (StarRank* p = course_array[count]->star_rank_head; p; p=p->next){
        if (p->student->sid == sid){
          if (p == course_array[count]->star_rank_head){
            if (p->next == nullptr){
              course_array[count]->star_rank_head = nullptr;
            }
            course_array[count]->star_rank_head = p->next;
            course_array[count]->stars_count[(p->star) -1] -=1;

            p->student->ranks_count -= 1;
            delete p;
            p = nullptr;
            return true;
            break;
          }
          else{
            prev->next = p->next;
            course_array[count]->stars_count[(p->star) -1] -=1;
            p->student->ranks_count -=1;
            delete p;
            p = nullptr;
            return true;
            break;
          }
        }
        prev = p;
      }

    }
    else{
      cout << "Failed to delete star_rank, star_rank not found in course "
          << course_id << endl;
      return false;
    }
  }
  return false;
}

//Takes as input a course_id and deletes the corresponding course.
//If the course does not exist, return false.
//Else if there are any star ranks, delete them, decrease the ranks_count of the corresponding students, and delete the course. Return true.
//If the course array has a size of N and N/2 entries are empty, decrease the array size by half (while maintaining the courses). Return true.

// ****IMPORTANT Notes****
// 1) Whenever you delete a class, you need to swap all the classes
// in the right of that class, one step left.
// For example, if we remove C4 from the array:
// [C1,C2,C3,C4,C5,C6,nullptr,nullptr]
// Then the resulting array will be:
// [C1,C2,C3,C5,C6,nullptr,nullptr,nullptr]

// 2) The minimum size of the array is 2.
// You MUST NOT reduce the size of the array to 1.

// @param: student_head points to the head of Student list
// @param: course_array the array of pointers for the Course
// @param: the id of the course the student ranks (course_id)
// @param: the number of courses in the site
// @out: a boolean value indicating whether the removal was successful
bool delete_course(Student *student_head, Course **&course_array,
                   const unsigned int course_id, unsigned int &num_courses) {
  // TODO: Write code to implement delete_course
  int count = 0;
  int new_num_courses = 0;
  bool ret_course = search_course(course_array, course_id, num_courses, count);
  if (ret_course == false){
    cout << "Failed to delete course, course " << course_id << " not found." << endl;
  }
  else{
    bool ret = true;
    do{
      if (course_array[count]->star_rank_head == nullptr){
        break;
      }
      else{
        ret = delete_star_rank(student_head, course_array, course_array[count]->star_rank_head->student->sid, course_id, num_courses);
      }
    } while (ret != false);
    delete course_array[count];
    //course_array[count] = nullptr;
    if (course_array[count+1] == nullptr){
      course_array[count] = nullptr;
      if ((count-1 < num_courses/2) && (num_courses>2)){
        num_courses = num_courses/2;
        cout << "reduce course array size to " << num_courses << endl;
      }
    }
    for (int i = count; (course_array[i] != nullptr) && (i< num_courses); i++){
      course_array[i] = course_array[i+1];
      if (course_array[i+2] == nullptr){
        course_array[i+1] = nullptr;
        if((i < num_courses/2) && (num_courses > 2)){
          num_courses = num_courses/2;
          cout << "reduce course array size to " << num_courses << endl;
          break;
        }
      }
    }

    return true;
  }
      // debugging
    for (int i = 0; i< num_courses; i++){
      if (course_array[i] != nullptr ){cout << course_array[i]->name <<" ->"; }
      else if (course_array[i] == nullptr) cout << "boom ->";
    }
  
  return false;
}

void clean_up(Student *&student_head, StarRank *&star_rank_head,
              Course **&course_array, unsigned int &num_courses) {
  // delete all courses, be carefully for the memory leak
  while (course_array[0] != nullptr) {
    delete_course(student_head, course_array, course_array[0]->course_id,
                  num_courses);
  }

  if (student_head != nullptr) {
    Student *student;
    while (student_head->next != nullptr) {
      // remove student, need first delete all the star_rank of this student, and
      // then delete star_ranks in course remove_student(student_head, star_rank_head,
      // student_head->sid);
      student = student_head->next;
      student_head->next = student_head->next->next;
      delete student;
    }
    delete student_head;
    student_head = nullptr;
  }

  delete[] course_array; // delete the dynamically allocated 2D array
  course_array = nullptr;
}

// Display the data of all students in increasing order of their sid in the form:
// === Student List ([sid, name, ranks_count]) ===
// [sid, name and ranks_count] -> [...] -> ... -> [...]
// @param: student_head points to the head of Student list
void display_students(Student *student_head) {
  cout << "=== Student List ([sid, name, star_rank count]) ===" << endl;
  
  // TODO: Write the code to display students

  if (student_head == nullptr){
    cout << "No items in the Student list" << endl; // Use this when no student exists
  }
  else{
      for (Student* p = student_head; p; p=p->next){
      cout <<"["<< p->sid <<", "<<p->name<<", "<<p->ranks_count<<"]";
      if (p->next != nullptr){
        cout << " -> ";
      }
    }  
    cout<< endl;
  }
}

//Display star ranks of the course given its id.
// In the form:
// star_ranks in course _COURSE_NAME_ : [star ranker sid : star] -> [...]

// @param: course_array the array of pointers for the Course
// @param: the number of courses in the site
// @param: the id of the course
void display_star_ranks(Course **course_array, const unsigned int num_courses,
                        const unsigned int course_id) {
  // TODO: Write the code to display star ranks
  // If the course exists but no star ranks exist yet,
  // use the following cout:
  // cout << "No StarRanks in the course " << __COURSE_NAME << endl; // __COURSE_NAME represents the name of the course
  int count = 0;
  bool ret_course = search_course(course_array, course_id, num_courses, count);
  if (ret_course == true){
    if (course_array[count]->star_rank_head == nullptr){
      cout << "star_ranks in course "<< course_array[count]->name<< " : " << "No StarRanks in the course " << course_array[count]->name<< endl; // __COURSE_NAME represents the name of the course
    }
    else{
      cout<< "star_ranks in course "<< course_array[count]->name<< " : ";
      for (StarRank* p = course_array[count]->star_rank_head; p; p=p->next){
        cout << "[" << p->student->sid<<": "<< p->star<<"]";
        if (p->next){
          cout << " -> ";
        }
      }
      cout<< endl;
      
    }
  }
  else{
    // If the course does not exist use the following cout:
    cout << "Course not found " << endl; // Display this if course not found
  }
}

//Display all existing courses. For each course display its name and its star-ranking array in the form:
// course_id : _COURSE_ID_, name : _COURSE_NAME, stars_count:
//*     _one_star
//**    _two_stars
//***   _three_stars
//****  _four_stars
//***** _five_stars
// @param: course_array the array of pointers for the Course
// @param: the number of courses in the site
void display_courses(Course **course_array, const unsigned int num_courses) {
  // TODO: Write the code to display students
  // If there is no course in the list, use the following cout:
  if (course_array[0] != nullptr){
    for (int i = 0; course_array[i] && i< num_courses; i++){
      cout<< "course_id : "<< course_array[i]->course_id<<", name : "<< course_array[i]->name<< ", stars_count : "<< endl
      <<"*     "<< course_array[i]->stars_count[0]<<endl<< "**    "<<course_array[i]->stars_count[1]<<endl<<"***   "<<course_array[i]->stars_count[2]<<endl<<"****  "
      <<course_array[i]->stars_count[3]<<endl <<"***** "<<course_array[i]->stars_count[4]<< endl;
    }
  }
  else{
    cout << "No course in the list " << endl;
  }
}

// === Region: The main function ===
// The main function implementation is given
// DO NOT make any changes to the main function
// ============================
int main() {
  enum MeunOption {
    OPTION_DISPLAY_STUDENT_LIST = 0,
    OPTION_DISPLAY_COURSES,
    OPTION_DISPLAY_COMMENTS,
    // OPTION_DISPLAY_CURRENT_LIST,
    OPTION_INSERT_STUDENT,
    OPTION_INSERT_COURSE,
    OPTION_INSERT_COMMENT,
    OPTION_DELETE_COMMENT,
    OPTION_DELETE_COURSE,
    OPTION_EXIT_SYSTEM,
    MAX_MENU_OPTIONS
  };
  const int MAX_MENU_OPTIONS_LENGTH = 80;
  char menu_options[MAX_MENU_OPTIONS][MAX_MENU_OPTIONS_LENGTH] = {
      "Display student list",
      "Display courses",
      "Display star_ranks of course ",
      "Insert a new student to the student list",
      "Insert a new course to the course list",
      "Insert a new star_rank to the star_rank list",
      "Delete a star_rank by student id and course id",
      "Delete a course by course id",
      "Exit the system"};

  Student *student_head = nullptr;
  StarRank *star_rank_head = nullptr;
  Course **course_array = nullptr;
  unsigned int num_courses = 0;
  int i, option;
  unsigned int sid, course_id, star;
  char name[MAX_TITLE] = "";
  bool ret = false;

  num_courses = 2;
  course_array = dynamic_init_course_array(num_courses);

  cout << "=== Simplified USTSPAC System ===" << endl;
  while (true) {
    cout << "=== Menu ===" << endl;
    for (i = 0; i < MAX_MENU_OPTIONS; i++)
      cout << i + 1 << ": " << menu_options[i]
           << endl; // shift by +1 when display

    cout << "Enter your option: " << endl;
    cin >> option;
    option = option - 1; // shift by -1 after entering the option

    // The invalid menu option handling
    if (option < 0 || option >= MAX_MENU_OPTIONS) {
      cout << "Invalid option" << endl;
      continue;
    }

    // Exit operations handling
    if (option == OPTION_EXIT_SYSTEM) {
      clean_up(student_head, star_rank_head, course_array, num_courses);
      break; // break the while loop
    }

    switch (option) {
    case OPTION_DISPLAY_STUDENT_LIST:
      display_students(student_head);
      break;
    case OPTION_DISPLAY_COURSES:
      display_courses(course_array, num_courses);
      break;
    case OPTION_DISPLAY_COMMENTS:
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }
      display_star_ranks(course_array, num_courses, course_id);

      break;
    case OPTION_INSERT_STUDENT:
      sid = 0;
      cout << "Enter the student id: " << endl;
      cin >> sid;
      if (sid <= 0) {
        cout << "Enter a valid student id > 0" << endl;
        break;
      }
      cout << "Enter a name: " << endl;
      cin >> name;

      ret = add_student(student_head, sid, name);
      if (ret == false) {
        cout << "Failed to insert student " << sid << endl;
      } else {
        cout << sid << " is successfully inserted" << endl;
      }
      break;
    case OPTION_INSERT_COURSE:
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }
      cout << "Enter a name: " << endl;
      cin >> name;

      ret = add_course(course_array, course_id, name, num_courses);
      if (ret == false) {
        cout << "Failed to insert course " << course_id << endl;
      } else {
        cout << course_id << " is successfully inserted" << endl;
      }
      break;
    case OPTION_INSERT_COMMENT:
      sid = 0;
      cout << "Enter the student id: " << endl;
      cin >> sid;
      if (sid <= 0) {
        cout << "Enter a valid student id > 0" << endl;
        break;
      }
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }
      star = 0;
      cout << "Enter the star rank of this course: " << endl;
      cin >> star;
      if (star <= 0 || star > MAX_RANKING_STARS) {
        cout << "Enter a valid course id > 0 && < " << MAX_RANKING_STARS
             << endl;
        break;
      }

      ret = add_star_rank(student_head, sid, course_array, course_id, num_courses,
                        star);
      if (ret == false) {
        cout << "Failed to insert star_rank, sid: " << sid
             << ", course id: " << course_id << endl;
      } else {
        cout << " star_rank is successfully inserted" << endl;
      }
      break;
    case OPTION_DELETE_COMMENT:
      sid = 0;
      cout << "Enter the student id: " << endl;
      cin >> sid;
      if (sid <= 0) {
        cout << "Enter a valid student id > 0" << endl;
        break;
      }
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }

      ret = delete_star_rank(student_head, course_array, sid, course_id,
                           num_courses);
      if (ret == false) {
        cout << "Failed to delete star_rank, sid: " << sid
             << ", course id: " << course_id << endl;
      } else {
        cout << " star_rank is successfully deleted" << endl;
      }
      break;
    case OPTION_DELETE_COURSE:
      course_id = 0;
      cout << "Enter the course id: " << endl;
      cin >> course_id;
      if (course_id <= 0) {
        cout << "Enter a valid course id > 0" << endl;
        break;
      }

      ret = delete_course(student_head, course_array, course_id, num_courses);
      if (ret == false) {
        cout << "Failed to delete course, course id: " << course_id << endl;
      } else {
        cout << "course is successfully deleted" << endl;
      }
      break;
    default:
      break;
    } // end of switch (option)
  }

  return 0;
}