#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable:4996)

#define MAX_LINE_LEN 100000
#define MAX_VERTICES 100

void CreateCleanFile();

void TrimTrailing();

void FindPointSize(int*, int*, int*);

void FindLineSize(int*, int*, int*);

void ArrayDimensioning(int, int, int, const int*, const int*, const int*);

void FillPointArrays();


typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    int n_vertices;
    Point vertices[MAX_VERTICES];
} Polygon;

Point** PolygonPoints;
Point** PassagePoints;
Point** PairPoints;

// Global file path variables
const char* mainFileName = "q1_input.txt";
const char* tempFileName = "temp_input_file.txt";
const char* cleanFileName = "clean_input_file.txt";

// Global delimiters
char* semiColonDelimiter;
char* commaDelimiter;
char* spaceDelimiter;

int main()
{
    char line[MAX_LINE_LEN];
    char* token;

    int polygonLineSize = 0;
    int passageLineSize = 0;
    int pairLineSize = 0;


    CreateCleanFile();
    FindLineSize(&polygonLineSize, &passageLineSize, &pairLineSize);

    int* polygonPointSize = (int*)malloc(polygonLineSize * sizeof(int));
    int* passagePointSize = (int*)malloc(passageLineSize * sizeof(int));
    int* pairsPointSize = (int*)malloc(pairLineSize * sizeof(int));

    FindPointSize(polygonPointSize, passagePointSize, pairsPointSize);

    ArrayDimensioning(polygonLineSize, passageLineSize, pairLineSize, polygonPointSize, passagePointSize,
        pairsPointSize);

    FillPointArrays();


    for (int t = 0; t < polygonLineSize; t++)
    {
        for (int z = 0; z < polygonPointSize[t]; z++)
        {
            printf("%f - %f\n", PolygonPoints[t][z].x, PolygonPoints[t][z].y);
        }
        printf("-----------------\n");
    }

    printf("\n\n");

    for (int t = 0; t < passageLineSize; t++)
    {
        for (int z = 0; z < passagePointSize[t]; z++)
        {
            printf("%f - %f\n", PassagePoints[t][z].x, PassagePoints[t][z].y);
        }
        printf("-----------------\n");
    }

    printf("\n\n");

    for (int t = 0; t < pairLineSize; t++)
    {
        for (int z = 0; z < pairsPointSize[t]; z++)
        {
            printf("%f - %f\n", PairPoints[t][z].x, PairPoints[t][z].y);
        }
        printf("-----------------\n");
    }

    return 0;
}


void FindPointSize(int* polygonPoints, int* passagePoints, int* pairsPoints)
{

    char line[MAX_LINE_LEN];
    char* token;
    int rowCounter = 0;
    int pointCounter = 0;
    int switchVariable = 0;
    int idx = 0;


    FILE* cleanFile = fopen(cleanFileName, "r");

    if (cleanFile == NULL)
    {
        perror("Error opening file");
        return;
    }


    while (fgets(line, MAX_LINE_LEN, cleanFile))
    {
        if (strcmp(line, "\n") == 0)
        {
            switchVariable++;
            idx = 0;
            continue;
        }

        switch (switchVariable)
        {
        case 0:
            token = strtok_s(line, " ", &spaceDelimiter);
            while (token != NULL)
            {
                pointCounter = pointCounter + 1;
                token = strtok_s(NULL, " ", &spaceDelimiter);
            }

            *(polygonPoints + idx) = pointCounter / 2;
            pointCounter = 0;
            idx = idx + 1;

            break;
        case 1:

            token = strtok_s(line, " ", &spaceDelimiter);
            while (token != NULL)
            {
                pointCounter = pointCounter + 1;
                token = strtok_s(NULL, " ", &spaceDelimiter);
            }

            *(passagePoints + idx) = pointCounter / 2;
            pointCounter = 0;
            idx = idx + 1;

            break;
        case 2:

            token = strtok_s(line, " ", &spaceDelimiter);
            while (token != NULL)
            {
                pointCounter = pointCounter + 1;
                token = strtok_s(NULL, " ", &spaceDelimiter);
            }

            *(pairsPoints + idx) = pointCounter / 2;
            pointCounter = 0;
            idx = idx + 1;

            break;
        default:
            break;
        }
    }
    idx = 0;
    fclose(cleanFile);
}

void FindLineSize(int* polygonLines, int* passageLines, int* pairLines)
{
    char line[MAX_LINE_LEN];
    int rowCounter = 0;
    int switchVariable = 0;

    FILE* cleanFile = fopen(cleanFileName, "r");

    if (cleanFile == NULL)
    {
        perror("Error opening file");
        return;
    }

    while (fgets(line, MAX_LINE_LEN, cleanFile))
    {
        if (strcmp(line, "\n") == 0)
        {
            switch (switchVariable)
            {
            case 0:
                *polygonLines = rowCounter;
                break;
            case 1:
                *passageLines = rowCounter;
                break;
            default:
                break;
            }
            switchVariable++;
            rowCounter = 0;
            continue;
        }

        rowCounter = rowCounter + 1;
    }
    *pairLines = rowCounter;

    fclose(cleanFile);


}

void CreateCleanFile()
{
    char line[MAX_LINE_LEN];
    char* token;
    char* subToken;

    // Open file in read mode
    FILE* mainFile = fopen(mainFileName, "r");
    FILE* tempFile = fopen(tempFileName, "w");

    if (mainFile == NULL)
    {
        perror("Error opening file");
        return;
    }

    while (fgets(line, MAX_LINE_LEN, mainFile))
    {
        line[strcspn(line, "\n")] = '\0';

        token = strtok_s(line, ";", &semiColonDelimiter);
        while (token != NULL)
        {
            subToken = strtok_s(token, ",", &commaDelimiter);
            while (subToken != NULL)
            {
                fprintf(tempFile, "%.4lf ", atof(subToken));
                subToken = strtok_s(NULL, ",", &commaDelimiter);
            }
            token = strtok_s(NULL, ";", &semiColonDelimiter);
        }
        fprintf(tempFile, "\n");

    }

    fclose(mainFile);
    fclose(tempFile);

    TrimTrailing();
    remove(tempFileName);
}

void TrimTrailing()
{
    int index;
    int i;
    char line[MAX_LINE_LEN];

    FILE* tempFile = fopen(tempFileName, "r");
    FILE* cleanFile = fopen(cleanFileName, "w");

    if (tempFile == NULL)
    {
        perror("Error opening file");
        return;
    }

    while (fgets(line, MAX_LINE_LEN, tempFile))
    {
        /* Set default index */
        index = -1;

        /* Find last index of non-white space character */
        i = 0;
        while (line[i] != '\0')
        {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
            {
                index = i;
            }

            i++;
        }

        /* Mark next character to last non-white space character as NULL */
        line[index + 1] = '\0';
        fprintf(cleanFile, "%s\n", line);

    }

    fclose(tempFile);
    fclose(cleanFile);
}

void ArrayDimensioning(int polygonLineSize, int passageLineSize, int pairLineSize,
    const int* polygonPointSize,
    const int* passagePointSize,
    const int* pairPointSize)
{

    PolygonPoints = (Point**)malloc(polygonLineSize * sizeof(Point*));
    for (int k = 0; k < polygonLineSize; k++)
    {
        PolygonPoints[k] = (Point*)malloc(polygonPointSize[k] * sizeof(Point));
    }

    PassagePoints = (Point**)malloc(passageLineSize * sizeof(Point*));
    for (int k = 0; k < passageLineSize; k++)
    {
        PassagePoints[k] = (Point*)malloc(passagePointSize[k] * sizeof(Point));
    }

    PairPoints = (Point**)malloc(pairLineSize * sizeof(Point*));
    for (int k = 0; k < pairLineSize; k++)
    {
        PairPoints[k] = (Point*)malloc(pairPointSize[k] * sizeof(Point));
    }
}

void FillPointArrays()
{
    char line[MAX_LINE_LEN];
    char* token;
    int switchArray = 0;
    int rowIdx = 0;
    int pointIdx = 0;

    FILE* cleanFile = fopen(cleanFileName, "r");
    if (cleanFile == NULL)
    {
        perror("Error opening file");
        return;
    }

    while (fgets(line, MAX_LINE_LEN, cleanFile))
    {
        if (strcmp(line, "\n") == 0)
        {
            switchArray++;
            rowIdx = 0;
            continue;
        }
        switch (switchArray)
        {
        case 0:
            token = strtok_s(line, " ", &spaceDelimiter);
            while (token != NULL)
            {
                PolygonPoints[rowIdx][pointIdx].x = atof(token);
                token = strtok_s(NULL, " ", &spaceDelimiter);
                PolygonPoints[rowIdx][pointIdx].y = atof(token);
                token = strtok_s(NULL, " ", &spaceDelimiter);

                pointIdx++;
            }
            rowIdx++;
            pointIdx = 0;

            break;

        case 1:
            token = strtok_s(line, " ", &spaceDelimiter);
            while (token != NULL)
            {
                PassagePoints[rowIdx][pointIdx].x = atof(token);
                token = strtok_s(NULL, " ", &spaceDelimiter);
                PassagePoints[rowIdx][pointIdx].y = atof(token);
                token = strtok_s(NULL, " ", &spaceDelimiter);

                pointIdx++;
            }
            rowIdx++;
            pointIdx = 0;

            break;

        case 2:
            token = strtok_s(line, " ", &spaceDelimiter);
            while (token != NULL)
            {
                PairPoints[rowIdx][pointIdx].x = atof(token);
                token = strtok_s(NULL, " ", &spaceDelimiter);
                PairPoints[rowIdx][pointIdx].y = atof(token);
                token = strtok_s(NULL, " ", &spaceDelimiter);

                pointIdx++;
            }
            rowIdx++;
            pointIdx = 0;

            break;

        default:
            break;
        }
    }
    fclose(cleanFile);
}