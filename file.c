#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#pragma warning(disable : 4996)

typedef struct studenttable { // структура для хранения книги
    char number[10]; // номер зачетной книжки
    char surname[25]; // фамилия студента
    char name[15]; // имя студента
    char namefather[25]; // отчество студента
    char fac[5]; // факультет студента
    char spec[30]; // специальность студента
} studenttable;

typedef struct booktable { // структура для хранения книги
    char isbn[20]; // номер ISBN
    char author[30]; // фио автора
    char title[25]; // название
    int total; // всего книг
    int available; // доступно книг
} booktable;

typedef struct usertable { // структура для хранения пользователей
    char login[15]; // номер зачетной книжки
    char password[15]; // фамилия студента
    int accessst; // доступ к БД со студентами
    int accessb; // доступ к БД с книгами
} usertable;

typedef struct studentbook { // структура для хранения пользователей
    char isbn[20]; // номер ISBN
    char number[10]; // номер зачетной книжки
    int day; // дата сдачи
    int mon; // месяц сдачи
    int year; // год сдачи
} studentbook;

// --------------------------Функции для работы с БД с книгами------------------------------------

booktable read_book() { // считывание книги
    booktable books; // создаём книгу
    printf("Введите ISBN (10 цифр): "); scanf("%[^\n]%*c", books.isbn);
    printf("Введите автора (И.О.Фамилия: "); scanf("%[^\n]%*c", books.author);
    printf("Введите название книги: "); scanf("%[^\n]%*c", books.title);
    printf("Введите общее кол-во книг: "); scanf("%d%*c", &books.total);
    printf("Введите имеющееся кол-во книг (должно быть меньше общего числа книг): "); scanf("%d%*c", &books.available);
    return books; // возвращаем считанную книгу
}

void printbook(booktable books) { // вывод книги
    printf("| %-10s ", books.isbn);
    printf("| %-30s ", books.author);
    printf("| %-25s ", books.title);
    printf("| %-5d ", books.total);
    printf("| %-7d ", books.available);
    printf("|\n");
}

void printtableb(booktable* books, int n) { // вывод библиотеки
    if (n == 0) { // если ещё нет книг, сообщаем об этом
        printf("Ни одной книги еще не добавлено\n");
        return;
    }

    printf("|    ISBN    |             Авторы             |       Название книг       | Всего | Имеется |\n");
    printf("|------------|--------------------------------|---------------------------|-------|---------|\n"); // выводим шапку таблицы
    for (int i = 0; i < n; i++)
        printbook(books[i]); //вывод каждой книги
}

booktable* addb(booktable* books, int* n) { // добавление новой книги
    printf("Введите информацию о новой книге\n");
    booktable abook = read_book(); // считываем запись

    for (int i = 0; i < *n; i++) {     // проверка на наличие копии по ISBN
        if (strcmp(books[i].isbn, abook.isbn) == 0) {
            printf("ISBN '%s' уже занят\n", abook.isbn);
            return books;
        }
    }

    books = (booktable*)realloc(books, (*n + 1) * sizeof(booktable));
    books[*n] = abook;
    (*n)++;

    return books; // возвращаем обновлённый массив
}

void delb(booktable* books, int* n, studentbook* stbs, int* l) { // удаление книги по isbn
    if (n == 0) {
        printf("Ни одной книги еще не добавлено\n");
        return;
    }

    char isbn[20];
    printf("Введите ISBN книги, которую хотите удалить: ");
    scanf("%[^\n]", &isbn); // считываем поле для поиска

    for (int i = 0; i < *l; i++) {
        if (strcmp(stbs[i].isbn, isbn) == 0) {
            printf("Данная книга имеется у кого-то из студентов в наличии, ее невозможно удалить\n");
            return;
        }
    }

    int flag = 0; // флаг для поиска совпадений
    int j = 0; // новый индекс для элементов после удаления

    for (int i = 0; i < *n; i++) {
        if (strcmp(books[i].isbn, isbn) != 0) {
            books[j++] = books[i];
        }
        else {
            flag = 1;
        }
    }

    if (flag = 1) {
        printf("Книги с ISBN '%s' не существует\n", isbn);
    }

    *n = j; // обновляем размер
}

void giveout(booktable* books, int n, studenttable* students, int* m, studentbook* stbs, int* l) { // выдача книги
    int exist = 0; //проверка на существование студента
    int dmin = 31, mmin = 12, ymin = 2030; 
    int stbssum, min;
    char isbn[20], number[10];
    printf("Введите ISBN книги, которую хотите получить: ");
    scanf("%[^\n]%*c", isbn);
    printf("Введите номер зачетной книжки студента, который получает книжку: ");
    scanf("%[^\n]%*c", number);

    for (int i = 0; i < n; i++) {
        if (strcmp(students[i].number, number) == 0) {
            exist++;
        }
    }
    if (exist == 0) {
        printf("Студента с таким номером не существует\n");
        return 0;
    }

    for (int i = 0; i < n; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) { // если нашли книгу
            if (books[i].available == 0) {
                for (int j = 0; j < *l; j++) {
                    if (strcmp(stbs[j].isbn, isbn) == 0) {
                        stbssum = stbs[j].year * 10000 + stbs[j].mon * 100 + stbs[j].day;
                        min = ymin * 10000 + mmin * 100 + dmin;
                        if (stbssum < min) {
                            dmin = stbs[j].day;
                            mmin = stbs[j].mon;
                            ymin = stbs[j].year;
                        }
                    }
                }
                printf("Данной книги нет в наличии. Ближайшая книга будет сдана %d.%d.%d\n", dmin, mmin, ymin);
            }
            else {
                books[i].available--;
                stbs = (studentbook*)realloc(stbs, (*l + 1) * sizeof(studentbook));
                strcpy(stbs[*l].isbn, isbn);
                strcpy(stbs[*l].number, number);
                stbs[*l].day = 31;
                stbs[*l].mon = 05;
                stbs[*l].year = 2020;
                (*l)++;
                printf("Книга успешно получена\n");
            }
            return 0;
        }
    }

    printf("Книги с ISBN '%s' не существует\n", isbn);
}

void returnbook(booktable* books, int n, studenttable* students, int* m, studentbook* stbs, int* l) { // возврат книги
    int exista = 0, existb = 0;
    char isbn[20], number[10];
    printf("Введите ISBN книги, которую хотите вернуть: ");
    scanf("%[^\n]%*c", isbn);
    printf("Введите номер зачетной книжки студента, который возвращает книжку: ");
    scanf("%[^\n]%*c", number);

    for (int i = 0; i < n; i++) {
        if (strcmp(students[i].number, number) == 0) {
            exista++;
        }
    }
    if (exista == 0) {
        printf("Студента с таким номером не существует\n");
        return 0;
    }
    for (int i = 0; i < *l; i++) {
        if ((strcmp(stbs[i].number, number) == 0) && (strcmp(stbs[i].isbn, isbn) == 0)) {
            existb++;
        }
    }
    if (existb == 0) {
        printf("Студент с таким номером не имеет в наличии такой книги\n");
        return 0;
    }


    for (int i = 0; i < n; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) { // если нашли книгу
            if (books[i].total == books[i].available) {
                printf("Невозможно вернуть эту книгу - все ее копии в наличии\n");
            }
            else {
                books[i].available++;
                int j = 0; // новый индекс для элементов после удаления
                for (int i = 0; i < *l; i++) {
                    if ((strcmp(stbs[i].isbn, isbn) != 0) || (strcmp(stbs[i].number, number) != 0)) {
                        stbs[j++] = stbs[i];
                    }
                }
                *l = j; // обновляем размер
                printf("Книга успешно возвращена\n");
            }
            return 0;
        }
    }

    printf("Книги с ISBN '%s' не существует\n", isbn);
}

void bookinfo(studenttable* students, int m, booktable* books, int n, studentbook* stbs, int l) {
    char isbn[20];
    int flag = 0;
    printf("Введите номер ISBN, чтобы просмотреть всю информацию о владельцах книги: ");
    scanf("%[^\n]%*c", isbn);

    printf("|   Номер   |             Фамилия             |       Имя       |         Отчество         | Факультет |         Специальность          |  Дата возврата  \n");
    printf("|-----------|---------------------------------|-----------------|--------------------------|-----------|--------------------------------|-----------------\n");

    for (int i = 0; i < m; i++) {
        if (strcmp(books[i].isbn,isbn) == 0) { // если нашли книгу
            for (int j = 0; j < l; j++) {
                if (strcmp(stbs[j].isbn, isbn) == 0) { // если нашли студента с книгой
                    for (int r = 0; r < n; r++) {
                        if (strcmp(students[r].number, stbs[j].number) == 0) { // поиск студента в таблице
                            printf("| %-9s ", students[r].number);
                            printf("| %-31s ", students[r].surname);
                            printf("| %-15s ", students[r].name);
                            printf("| %-24s ", students[r].namefather);
                            printf("| %-9s ", students[r].fac);
                            printf("| %-30s ", students[r].spec);
                            printf("| %d.%d.%d ", stbs[j].day, stbs[j].mon, stbs[j].year);
                            printf("\n");
                        }
                    }
                }
            }
            flag++;
        }
    }
    if (flag == 0) {
        printf("Книги с ISBN '%s' не существует\n", isbn);
    }
}

void savetableb(char* tableb, booktable* books, int n) { // сохранение книг в файл 
    if (n == 0) {
        printf("Ни одной книги еще не добавлено\n");
        return 0;
    }

    FILE* fileb = fopen(tableb, "w"); // открываем файл на запись

    for (int i = 0; i < n; i++) {
        fprintf(fileb, "%s;", books[i].isbn);
        fprintf(fileb, "%s;", books[i].author);
        fprintf(fileb, "%s;", books[i].title);
        fprintf(fileb, "%d;", books[i].total);
        fprintf(fileb, "%d", books[i].available);

        if (i < n - 1)
            fprintf(fileb, "\n");
    }

    fclose(fileb); // закрываем файл
}

booktable* loadtableb(char* tableb, int* n) { // загрузка книг из файла
    FILE* fileb = fopen(tableb, "r"); // открываем файл на чтение

    if (fileb == 0) { // если не удалось открыть файл
        printf("Не удалось открыть файл '%s'\n", tableb);
        *n = 0;
        return NULL; // возвращаем пустой указатель
    }

    booktable* books = (booktable*)malloc(sizeof(booktable)); // выделяем память под один элемент
    *n = 0; // изначально элементов нет

    while (feof(fileb) == 0) {
        fscanf(fileb, "%[^;]%*c", books[*n].isbn);
        fscanf(fileb, "%[^;]%*c", books[*n].author);
        fscanf(fileb, "%[^;]%*c;", books[*n].title);
        fscanf(fileb, "%d%*c", &books[*n].total);
        fscanf(fileb, "%d%*c", &books[*n].available);
        *n = *n + 1; // увеличиваем размер
        books = (booktable*)realloc(books, (*n + 1) * sizeof(booktable)); // перераспределяем память
    }

    fclose(fileb);
    return books; // возвращаем считанный массив
}

// ------------------------Функции для работы с БД со студентами----------------------------------

void printstudent(studenttable students) { // вывод студента
    printf("| %-9s ", students.number);
    printf("| %-31s ", students.surname);
    printf("| %-15s ", students.name);
    printf("| %-24s ", students.namefather);
    printf("| %-9s ", students.fac);
    printf("| %-30s ", students.spec);
    printf("|\n");
}

void printtablest(studenttable* students, int m) { // вывод студентов
    if (m == 0) { // если ещё нет студентов, сообщаем об этом
        printf("Ни одного студента еще не добавлено\n");
        return;
    }

    printf("|   Номер   |             Фамилия             |       Имя       |         Отчество         | Факультет |         Специальность          |\n");
    printf("|-----------|---------------------------------|-----------------|--------------------------|-----------|--------------------------------|\n"); // выводим шапку таблицы
    for (int i = 0; i < m; i++)
        printstudent(students[i]); //вывод каждого студента
}

studenttable read_student() { // чтение новой информации о студенте
    studenttable students; // введение нового студента
    printf("Введите номер зачетной книжки: "); scanf("%[^\n]%*c", students.number);
    printf("Введите фамилию студента: "); scanf("%[^\n]%*c", students.surname);
    printf("Введите имя студента: "); scanf("%[^\n]%*c", students.name);
    printf("Введите отчество студента: "); scanf("%[^\n]%*c", students.namefather);
    printf("Введите факультет студента: "); scanf("%[^\n]%*c", students.fac);
    printf("Введите специальность студента: "); scanf("%[^\n]%*c", students.spec);
    return students; // возвращаем введенного студента
}

studenttable* addstudent(studenttable* students, int* m) { // добавление новой книги
    printf("Введите информацию о новом студенте\n");
    studenttable astudent = read_student(); // считываем запись

    for (int i = 0; i < *m; i++) {     // проверка на наличие копии по номеру зачетной книжки
        if (!strcmp(students[i].number, astudent.number)) {
            printf("Студент с номером '%s' уже имеется\n", astudent.number);
            return students;
        }
    }

    students = (studenttable*)realloc(students, (*m + 1) * sizeof(studenttable));
    students[*m] = astudent;
    (*m)++;

    return students; // возвращаем обновлённый массив
}

void delstudent(studenttable* students, int* m, studentbook* stbs, int* l) { // удаление книги по isbn
    if (m == 0) {
        printf("Ни одного студента еще не добавлено\n");
        return;
    }

    char number[10];
    printf("Введите номер зачетной книжки студента, которого хотите удалить: ");
    scanf("%[^\n]%*c", &number); // считываем поле для поиска

    for (int i = 0; i < *l; i++) {
        if (strcmp(stbs[i].number, number) == 0) {
            printf("Данный студент имеет книгу, его невозможно удалить\n");
            return;
        }
    }

    int flagst = 0; // флаг для поиска совпадений
    int j = 0; // новый индекс для элементов после удаления

    for (int i = 0; i < *m; i++) {
        if (strcmp(students[i].number, number) != 0) {
            students[j++] = students[i];
        }
        else {
            flagst = 1;
        }
    }

    if (flagst == 0) {
        printf("Студента с номером '%s' не существует\n", number);
    }

    *m = j; // обновляем размер
}

void findstudent(studenttable* students, int m) { // поиск студента
    char surname[25];
    int flagst = 0;
    printf("Введите фамилию, когорого хотите найти: ");
    scanf("%[^\n]%*c", surname);

    printf("|   Номер   |             Фамилия             |       Имя       |         Отчество         | Факультет |         Специальность          |\n");
    printf("|-----------|---------------------------------|-----------------|--------------------------|-----------|--------------------------------|\n");

    for (int i = 0; i < m; i++) {
        if (strcmp(students[i].surname, surname) == 0) { // если нашли студента
            printstudent(students[i]);
            flagst++;
        }
    }
    if (flagst == 0) {
        printf("Студента с фамилией '%s' не существует\n", surname);
    }
}

void createbackupst(studenttable* students, int m) { // бэкап студентов в файл 
    char backupname[50] = { 0 };

    if (m == 0) {
        printf("Ни одного студента еще не добавлено\n");
        return 0;
    }

    struct tm* t; 
    time_t timer = time(NULL); 
    t = localtime(&timer); // перевод в время МСК и добавляет все в структуру
    strftime(backupname, 50, "students_%d.%m.%Y; %H:%M:%S.csv", t);
    FILE* filest = fopen(backupname, "w");
    if (filest == 0) { // если не удалось открыть файл
        printf("Не удалось создать файл '%s'\n", backupname);
        return NULL; // возвращаем пустой указатель
    }

    for (int i = 0; i < m; i++) {
        fprintf(filest, "%s;", students[i].number);
        fprintf(filest, "%s;", students[i].surname);
        fprintf(filest, "%s;", students[i].name);
        fprintf(filest, "%s;", students[i].namefather);
        fprintf(filest, "%s;", students[i].fac);
        fprintf(filest, "%s", students[i].spec);

        if (i < m - 1)
            fprintf(filest, "\n");
    }
    printf("Бэкап создан\n");
    fclose(filest);
}

studenttable* loadbackupst(char* tablest, int* m) { // загрузка студентов из файла
    char backupname[20];
    printf("Введите название файла бэкапа, который вы хотите загрузить: ");
    scanf("%[^\n]%*c", backupname);

    FILE* filest = fopen(backupname, "r");

    if (filest == 0) { // если не удалось открыть файл
        printf("Не удалось открыть файл '%s'\n", backupname);
        *m = 0;
        return NULL; // возвращаем пустой указатель
    }

    studenttable* students = (studenttable*)malloc(sizeof(studenttable)); // выделяем память под один элемент
    *m = 0; // изначально элементов нет

    while (feof(filest) == 0) {
        fscanf(filest, "%[^;]%*c", students[*m].number);
        fscanf(filest, "%[^;]%*c", students[*m].surname);
        fscanf(filest, "%[^;]%*c;", students[*m].name);
        fscanf(filest, "%[^;]%*c", students[*m].namefather);
        fscanf(filest, "%[^;]%*c", students[*m].fac);
        fscanf(filest, "%[^\n]%*c;", students[*m].spec);
        *m = *m + 1; // увеличиваем размер
        students = (studenttable*)realloc(students, (*m + 1) * sizeof(studenttable)); // перераспределяем память
    }

    fclose(filest);
    printf("Файл открыт\n");
    return students; // возвращаем считанный массив
}

void studentinfo(studenttable* students, int m, booktable* books, int n, studentbook* stbs, int l) {
    char number[10];
    int flag = 0;
    printf("Введите номер зачетной книжки, чтобы просмотреть всю информацию о ваших книгах: ");
    scanf("%[^\n]%*c", number);

    printf("|    ISBN    |             Авторы             |       Название книг       |  Дата возврата  \n");
    printf("|------------|--------------------------------|---------------------------|-----------------\n"); 

    for (int i = 0; i < m; i++) {
        if (strcmp(students[i].number, number) == 0) { // если нашли студента
            for (int j = 0; j < l; j++) {
                if (strcmp(stbs[j].number, number) == 0) { // если нашли книгу у студента
                    for (int r = 0; r < n; r++) {
                        if (strcmp(books[r].isbn, stbs[j].isbn) == 0) { // поиск книги в библиотеке
                            printf("| %-10s ", books[r].isbn);
                            printf("| %-30s ", books[r].author);
                            printf("| %-25s ", books[r].title);
                            printf("| %d.%d.%d ", stbs[j].day, stbs[j].mon, stbs[j].year);
                            printf("\n");
                        }
                    }
                }
            }
            flag++;
        }
    }
    if (flag == 0) {
        printf("Студента с номером '%s' не существует\n", number);
    }
}

void savetablest(char* tablest, studenttable* students, int m) { // сохранение студентов в файл 
    if (m == 0) {
        printf("Ни одного студента еще не добавлено\n");
        return 0;
    }

    FILE* filest = fopen(tablest, "w");

    for (int i = 0; i < m; i++) {
        fprintf(filest, "%s;", students[i].number);
        fprintf(filest, "%s;", students[i].surname);
        fprintf(filest, "%s;", students[i].name);
        fprintf(filest, "%s;", students[i].namefather);
        fprintf(filest, "%s;", students[i].fac);
        fprintf(filest, "%s", students[i].spec);

        if (i < m - 1)
            fprintf(filest, "\n");
    }

    fclose(filest);
}

studenttable* loadtablest(char* tablest, int* m) { // загрузка студентов из файла
    FILE* filest = fopen(tablest, "r");

    if (filest == 0) { // если не удалось открыть файл
        printf("Не удалось открыть файл '%s'\n", tablest);
        *m = 0;
        return NULL; // возвращаем пустой указатель
    }

    studenttable* students = (studenttable*)malloc(sizeof(studenttable)); // выделяем память под один элемент
    *m = 0; // изначально элементов нет

    while (feof(filest) == 0) {
        fscanf(filest, "%[^;]%*c", students[*m].number);
        fscanf(filest, "%[^;]%*c", students[*m].surname);
        fscanf(filest, "%[^;]%*c", students[*m].name);
        fscanf(filest, "%[^;]%*c", students[*m].namefather);
        fscanf(filest, "%[^;]%*c", students[*m].fac);
        fscanf(filest, "%[^\n]%*c", students[*m].spec);
        *m = *m + 1; // увеличиваем размер
        students = (studenttable*)realloc(students, (*m + 1) * sizeof(studenttable)); // перераспределяем память
    }

    fclose(filest);
    return students; // возвращаем считанный массив
}

// -------------------------Работа с логином/паролем-----------------------------------

usertable* loadtableuser(char* tableuser, int* k) { // загрузка пользователей из файла
    FILE* fileuser = fopen(tableuser, "r");

    if (fileuser == 0) { // если не удалось открыть файл
        printf("Не удалось открыть файл '%s'\n", tableuser);
        *k = 0;
        return NULL; // возвращаем пустой указатель
    }

    usertable* users = (usertable*)malloc(sizeof(usertable)); // выделяем память под один элемент
    *k = 0; // изначально элементов нет

    while (feof(fileuser) == 0) {
        fscanf(fileuser, "%[^;]%*c", users[*k].login);
        fscanf(fileuser, "%[^;]%*c;", users[*k].password);
        fscanf(fileuser, "%d%*c", &users[*k].accessst);
        fscanf(fileuser, "%d%*c", &users[*k].accessb);
        *k = *k + 1; // увеличиваем размер
        users = (usertable*)realloc(users, (*k + 1) * sizeof(usertable)); // перераспределяем память
    }

    fclose(fileuser);
    return users; // возвращаем считанный массив
}

// -------------------------Работа с student_books.csv-----------------------------------

studentbook* loadstb(char* stb, int* l) { // загрузка взятых книг из файла
    FILE* filestb = fopen(stb, "r");

    if (filestb == 0) { // если не удалось открыть файл
        printf("Не удалось открыть файл '%s'\n", stb);
        *l = 0;
        return NULL; // возвращаем пустой указатель
    }

    studentbook* stbs = (studentbook*)malloc(sizeof(studentbook)); // выделяем память под один элемент
    *l = 0; // изначально элементов нет

    while (feof(filestb) == 0) {
        fscanf(filestb, "%[^;]%*c", stbs[*l].isbn);
        fscanf(filestb, "%[^;]%*c;", stbs[*l].number);
        fscanf(filestb, "%d%*c", &stbs[*l].day);
        fscanf(filestb, "%d%*c", &stbs[*l].mon);
        fscanf(filestb, "%d%*c", &stbs[*l].year);
        *l = *l + 1; // увеличиваем размер
        stbs = (studentbook*)realloc(stbs, (*l + 1) * sizeof(studentbook)); // перераспределяем память
    }

    fclose(filestb);
    return stbs; // возвращаем считанный массив
}

void savestb(char* stb, studentbook* stbs, int l) { // сохранение взятых книг в файл 
    FILE* filestb = fopen(stb, "w");

    for (int i = 0; i < l; i++) {
        fprintf(filestb, "%s;", stbs[i].isbn);
        fprintf(filestb, "%s;", stbs[i].number);
        fprintf(filestb, "%d.", stbs[i].day);
        fprintf(filestb, "%d.", stbs[i].mon);
        fprintf(filestb, "%d", stbs[i].year);

        if (i < l - 1)
            fprintf(filestb, "\n");
    }

    fclose(filestb);
}

// ----------------------------Логирование действий--------------------------------------

void logging(char* log, char* func) {
    FILE* logging = fopen("library.log", "a");
    time_t s_time;
    struct tm* m_time;
    char timer[100];
    s_time = time(NULL);
    m_time = localtime(&s_time);
    strftime(timer, 128, "%d.%m.%y %H:%M:%S", m_time);
    fprintf(logging, "%s;%s;%s", timer, log, func);
    fprintf(logging, "\n");
    fclose(logging);
}

// --------------------------------------------------------------------------------------

int main() {
    int m = 0, n = 0, k = 0, l = 0; // количество элементов в массиве
    char log[15], pass[15]; // логин и пароль
    studenttable* students = loadtablest("students.csv", &m); // массив студентов
    booktable* books = loadtableb("books.csv", &n); // массив книг
    usertable* users = loadtableuser("users.csv", &k); // массив пользователей
    studentbook* stbs = loadstb("student_books.csv", &l); // массив пользователей
    int lvl = 0; //уровень доступа к БД (1 - students, 2 - books)
    int admlvl = 0; //уровень доступа к двум БД
    int punktst, punktb, punktadm; // выбираемый пункт меню
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    printf("Введите логин: ");
    scanf("%[^\n]%*c", log); // считываем логин
    printf("Введите пароль: ");
    scanf("%[^\n]%*c", pass); // считываем пароль
    printf("\n");
    for (int i = 0; i < k; i++) {
        if ((strcmp(users[i].login, log) == 0) && (strcmp(users[i].password, pass) == 0) && (users[i].accessst == 1) && (users[i].accessb == 0)) { // доступ только к БД со студентами
            lvl = 1;
            break;
        }
        else if ((strcmp(users[i].login, log) == 0) && (strcmp(users[i].password, pass) == 0) && (users[i].accessst == 0) && (users[i].accessb == 1)) {
            lvl = 2;
            break;
        }
        else if ((strcmp(users[i].login, log) == 0) && (strcmp(users[i].password, pass) == 0) && (users[i].accessst == 1) && (users[i].accessb == 1)) {
            admlvl = 1;
            break;
        }
    }
    if ((lvl == 0) && (admlvl == 0)) {
        printf("Такого пользователя не существует\n");
        free(users);
        free(students);
        free(books);
        free(stbs);
        return 0;
    }

    while (1) {
        if ((admlvl == 1) && (lvl == 0)) {
            printf("Меню доступа к базам данных\n");
            printf("1. База данных студентов студентов\n");
            printf("2. База данных библиотеки\n");
            printf("Введите базу данных > ");
            scanf("%d%*c", &punktadm); // считываем пункт меню

            while ((punktadm < 1) || (punktadm > 2)) { // проверка на верность введенного пункта меню
                printf("Неверный пункт меню, попробуйте еще раз > ");
                scanf("%d%*c", &punktadm);
            }
            switch (punktadm) {
            case 1: lvl = 1;
                logging(log, "gotostmenu");
                break;
            case 2: lvl = 2;
                logging(log, "gotobmenu");
                break;
            }
            printf("\n");
        }
        if (lvl == 1) {
            do {
                printf("Меню списка студентов\n");
                printf("1. Вывод списка студентов\n");
                printf("2. Добавить нового студента\n");
                printf("3. Удалить студента\n");
                printf("4. Бэкап базы\n");
                printf("5. Восстановление базы из бэкапа\n");
                printf("6. Поиск по фамилии студента\n");
                printf("7. Информация о книгах студента\n");
                printf("8. Сохранить и выйти\n");
                if (admlvl == 1) {
                    printf("0. Выйти к меню с выбором БД\n");
                }
                printf("Введите пункт меню > ");
                scanf("%d%*c", &punktst); // считываем пункт меню

                while (((punktst < 1) || (punktst > 8)) && (punktst != 0)) { // проверка на верность введенного пункта меню
                    printf("Неверный пункт меню, попробуйте еще раз > ");
                    scanf("%d%*c", &punktst);
                }

                switch (punktst) {
                case 1: 
                    printtablest(students, m);
                    logging(log, "printtablest");
                    break;
                case 2: 
                    students = addstudent(students, &m);
                    logging(log, "addstudent");
                    break;
                case 3: 
                    delstudent(students, &m, stbs, &l);
                    logging(log, "delstudent");
                    break;
                case 4: 
                    createbackupst(students, m);
                    logging(log, "createbackupst");
                    break;
                case 5: 
                    loadbackupst(students, &m);
                    logging(log, "loadbackupst");
                    break;
                case 6: 
                    findstudent(students, m);
                    logging(log, "findstudent");
                    break;
                case 7:
                    studentinfo(students, m, books, n, stbs, l);
                    logging(log, "studentinfo");
                    break;
                case 8: 
                    savetablest("students.csv", students, m); // сохраняем студентов в файл
                    savestb("student_books.csv", stbs, l);
                    logging(log, "savetablest");
                    free(users);
                    free(students);
                    free(books);
                    free(stbs); // освобождаем динамическую память
                    return 0;
                }
                printf("\n");
            } while (punktst != 0); // повторяем, пока не решим выйти
            logging(log, "backtomainmenu");
            lvl = 0;
        }
        if (lvl == 2) {
            do {
                printf("Приветствуем в библиотеке!\n");
                printf("1. Вывод библиотеки\n");
                printf("2. Добавить новую книгу в библиотеку\n");
                printf("3. Удалить книгу из библиотеки\n");
                printf("4. Получение книги\n");
                printf("5. Возврат книги\n");
                printf("6. Информация о владельцах книги\n");
                printf("7. Сохранить и выйти\n");
                if (admlvl == 1) {
                    printf("0. Выйти к меню с выбором БД\n");
                }
                printf("Введите пункт меню > ");
                scanf("%d%*c", &punktb); // считываем пункт меню

                while (((punktb < 1) || (punktb > 7)) && (punktb != 0)) { // проверка на верность введенного пункта меню
                    printf("Неверный пункт меню, попробуйте еще раз: ");
                    scanf("%d%*c", &punktb);
                }

                switch (punktb) {
                case 1: 
                    printtableb(books, n);
                    logging(log, "printtableb");
                    break;
                case 2: 
                    books = addb(books, &n);
                    logging(log, "addb");
                    break;
                case 3: 
                    delb(books, &n, stbs, &l);
                    logging(log, "delb");
                    break;
                case 4: 
                    giveout(books, n, students, &m, stbs, &l);
                    logging(log, "giveout");
                    break;
                case 5: 
                    returnbook(books, n, students, &m, stbs, &l);
                    logging(log, "returnbook");
                    break;
                case 6:
                    bookinfo(students, m, books, n, stbs, l);
                    logging(log, "bookinfo");
                    break;
                case 7: 
                    savetableb("books.csv", books, n); // сохраняем книги в файл
                    savestb("student_books.csv", stbs, l);
                    logging(log, "savetableb");
                    free(users);
                    free(students);
                    free(books);
                    free(stbs); // освобождаем динамическую память
                    return 0;
                }
                printf("\n");
            } while (punktb != 0); // повторяем, пока не решим выйти
            logging(log, "backtomainmenu");
            lvl = 0;
        }
    }
}
