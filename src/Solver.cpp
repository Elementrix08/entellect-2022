#include "Solver.h"
#include <iostream>

using namespace std;

Solver::Solver() {
    cin >> stepAllowance;

    cin >> numCoal;
    coalPoints = vector<Point>(numCoal);

    for (int i = 0; i < numCoal; ++i)
        cin >> coalPoints[i].x >> coalPoints[i].y;

    cin >> numFish;
    fishPoints = vector<Point>(numFish);

    for (int i = 0; i < numFish; ++i)
        cin >> fishPoints[i].x >> fishPoints[i].y;

    cin >> numMetal;
    metalPoints = vector<Point>(numMetal);

    for (int i = 0; i < numMetal; ++i)
        cin >> metalPoints[i].x >> metalPoints[i].y;

    for (int i = 0; i < NUM_RESOURCES; ++i)
        cin >> quotaAmounts[i];

    cin >> quotaMultiplier;

    cin >> numRows >> numCols;
    map = vector<vector<string>>(numRows, vector<string>(numCols));

    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numCols; ++c)
            cin >> map[r][c];
    }
}

void Solver::toString() {
    printf("Step Allowance: %d\n", stepAllowance);
    printf("Quotas: %d %d %d\n", quotaAmounts[0], quotaAmounts[1],
           quotaAmounts[2]);
    printf("Quota multiplier %f\n", quotaMultiplier);
    printf("Coal points: ");

    for (int i = 0; i < numCoal; ++i)
        cout << coalPoints[i].toString() << " ";

    printf("\n");

    printf("Fish points: ");

    for (int i = 0; i < numFish; ++i)
        cout << fishPoints[i].toString() << " ";

    printf("\n");

    printf("Metal points: ");

    for (int i = 0; i < numMetal; ++i)
        cout << metalPoints[i].toString() << " ";

    printf("\n");

    printf("Map size: %d x %d\n", numRows, numCols);

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            cout << map[i][j] << " ";
        }

        printf("\n");
    }
}