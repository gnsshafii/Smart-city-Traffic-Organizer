/*
===============================================================================
                 SMART CITY TRAFFIC OPTIMIZER
                 CSE Algorithm Lab Project

Project Focus:
    Smart traffic management using important Algorithm Lab topics.

Algorithms:
    1. Greedy-style Traffic Signal Allocation
    2. Merge Sort       -> Traffic Ranking
    3. BFS              -> Network Exploration
    4. Dijkstra         -> Fastest Route
    5. Prim's MST       -> Minimum Road Network
    6. Kruskal's MST    -> Minimum Road Network

Data Structures:
    - Structures
    - Arrays
    - Adjacency Matrix
    - Queue
    - Edge List
    - Parent Array
    - Disjoint Set / Union-Find

Language:
    Standard C

Compatible with:
    GCC, Code::Blocks, Dev-C++, VS Code + C compiler

===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 6
#define NAME_LEN 40
#define INF 999999

#define CYCLE_TIME 120
#define YELLOW_TIME 4
#define MIN_GREEN 10

/* ========================= DATA STRUCTURES ========================= */

typedef struct {
    char name[NAME_LEN];

    int north;
    int south;
    int east;
    int west;

    int total;
    int nsGreen;
    int ewGreen;
    int priority;
} Intersection;

typedef struct {
    int u;
    int v;
    int weight;
} Edge;

/* ========================= GLOBAL DATA ========================= */

Intersection city[MAX_NODES];
int graph[MAX_NODES][MAX_NODES];
int nodeCount = MAX_NODES;

/* ========================= DISPLAY FUNCTIONS ========================= */

void line(char ch, int n) {
    int i;

    for (i = 0; i < n; i++)
        putchar(ch);

    putchar('\n');
}

void title(const char *text) {
    printf("\n");
    line('=', 68);
    printf("              SMART CITY TRAFFIC OPTIMIZER\n");
    line('=', 68);
    printf("  %s\n", text);
    line('-', 68);
}

void pauseScreen(void) {
    char buffer[8];

    printf("\n  Press ENTER to continue...");
    fgets(buffer, sizeof(buffer), stdin);
}

/* ========================= GRAPH INITIALIZATION ========================= */

void initializeGraph(void) {
    int i, j;

    for (i = 0; i < MAX_NODES; i++) {
        for (j = 0; j < MAX_NODES; j++) {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }
}

/* ========================= GREEDY-STYLE ALGORITHM =========================
   More traffic in one direction receives a larger share of green time.
   This is a practical greedy-style allocation used for the simulation.
============================================================================ */

void optimizeSignal(Intersection *x) {
    int availableGreen;
    int nsTraffic;
    int ewTraffic;

    availableGreen = CYCLE_TIME - (2 * YELLOW_TIME);

    nsTraffic = x->north + x->south;
    ewTraffic = x->east + x->west;

    x->total = nsTraffic + ewTraffic;

    if (x->total == 0) {
        x->nsGreen = availableGreen / 2;
        x->ewGreen = availableGreen - x->nsGreen;
    } else {
        x->nsGreen = (nsTraffic * availableGreen) / x->total;
        x->ewGreen = availableGreen - x->nsGreen;
    }

    /* Minimum green time for both directions */
    if (x->nsGreen < MIN_GREEN) {
        x->nsGreen = MIN_GREEN;
        x->ewGreen = availableGreen - MIN_GREEN;
    }

    if (x->ewGreen < MIN_GREEN) {
        x->ewGreen = MIN_GREEN;
        x->nsGreen = availableGreen - MIN_GREEN;
    }

    if (x->total >= 60)
        x->priority = 1;          /* HIGH */
    else if (x->total >= 25)
        x->priority = 2;          /* MEDIUM */
    else
        x->priority = 3;          /* LOW */
}

void optimizeAll(void) {
    int i;

    for (i = 0; i < nodeCount; i++)
        optimizeSignal(&city[i]);
}

/* ========================= SAMPLE CITY ========================= */

void loadSampleCity(void) {
    const char *names[MAX_NODES] = {
        "Central Square",
        "North Gate",
        "East Market",
        "West Station",
        "South Park",
        "University Gate"
    };

    int traffic[MAX_NODES][4] = {
        {28, 22, 18, 12},
        {15, 12, 10,  8},
        {22, 18, 16, 10},
        {10,  9, 20, 17},
        {18, 14,  8,  7},
        {25, 20, 12, 10}
    };

    int i;

    for (i = 0; i < nodeCount; i++) {
        strcpy(city[i].name, names[i]);

        city[i].north = traffic[i][0];
        city[i].south = traffic[i][1];
        city[i].east  = traffic[i][2];
        city[i].west  = traffic[i][3];

        optimizeSignal(&city[i]);
    }

    /*
       Weighted undirected road network.
       Weight = estimated travel time in minutes.
    */

    graph[0][1] = graph[1][0] = 4;
    graph[0][2] = graph[2][0] = 3;
    graph[0][3] = graph[3][0] = 5;
    graph[0][4] = graph[4][0] = 6;

    graph[1][2] = graph[2][1] = 2;
    graph[1][5] = graph[5][1] = 5;

    graph[2][5] = graph[5][2] = 4;

    graph[3][4] = graph[4][3] = 2;
    graph[3][5] = graph[5][3] = 7;

    graph[4][5] = graph[5][4] = 3;
}

/* ========================= DASHBOARD ========================= */

void showDashboard(void) {
    int i;

    title("TRAFFIC & SIGNAL DASHBOARD");

    optimizeAll();

    printf("  %-21s %-7s %-10s %-10s %-9s\n",
           "Intersection", "Traffic", "N-S Green",
           "E-W Green", "Priority");

    line('-', 68);

    for (i = 0; i < nodeCount; i++) {
        const char *priority;

        if (city[i].priority == 1)
            priority = "HIGH";
        else if (city[i].priority == 2)
            priority = "MEDIUM";
        else
            priority = "LOW";

        printf("  %-21s %-7d %-10ds %-10ds %-9s\n",
               city[i].name,
               city[i].total,
               city[i].nsGreen,
               city[i].ewGreen,
               priority);
    }

    printf("\n  Signal cycle: %d seconds\n", CYCLE_TIME);
    printf("  Yellow time : %d seconds per phase\n", YELLOW_TIME);
    printf("  Green time is allocated according to traffic volume.\n");
}

/* ========================= MERGE SORT ========================= */

int comesBefore(Intersection a, Intersection b) {
    if (a.total != b.total)
        return a.total > b.total;

    return strcmp(a.name, b.name) < 0;
}

void merge(Intersection arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int i, j, k;

    Intersection *L;
    Intersection *R;

    L = (Intersection *)malloc(n1 * sizeof(Intersection));
    R = (Intersection *)malloc(n2 * sizeof(Intersection));

    if (L == NULL || R == NULL) {
        free(L);
        free(R);
        return;
    }

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (comesBefore(L[i], R[j]))
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(Intersection arr[], int left, int right) {
    int mid;

    if (left >= right)
        return;

    mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}

void rankTraffic(void) {
    Intersection temp[MAX_NODES];
    int i;

    title("TRAFFIC RANKING - MERGE SORT");

    optimizeAll();

    for (i = 0; i < nodeCount; i++)
        temp[i] = city[i];

    mergeSort(temp, 0, nodeCount - 1);

    printf("\n  Rank  Intersection             Vehicles   Priority\n");
    line('-', 58);

    for (i = 0; i < nodeCount; i++) {
        const char *priority;

        if (temp[i].priority == 1)
            priority = "HIGH";
        else if (temp[i].priority == 2)
            priority = "MEDIUM";
        else
            priority = "LOW";

        printf("  %-5d %-24s %-10d %s\n",
               i + 1,
               temp[i].name,
               temp[i].total,
               priority);
    }

    printf("\n  Algorithm: Merge Sort\n");
    printf("  Complexity: O(n log n)\n");
}

/* ========================= BFS =========================
   Uses a queue to explore the road network level by level.
============================================================================ */

void bfs(void) {
    int start;
    int visited[MAX_NODES] = {0};
    int queue[MAX_NODES];

    int front = 0;
    int rear = 0;

    int u, v;

    title("NETWORK EXPLORATION - BFS");

    printf("\n  Select a starting intersection:\n");

    for (v = 0; v < nodeCount; v++)
        printf("  [%d] %s\n", v + 1, city[v].name);

    start = 0;

    while (1) {
        char buffer[50];
        int choice;

        printf("\n  Start (1-%d): ", nodeCount);

        if (fgets(buffer, sizeof(buffer), stdin) &&
            sscanf(buffer, "%d", &choice) == 1 &&
            choice >= 1 && choice <= nodeCount) {
            start = choice - 1;
            break;
        }

        printf("  Invalid choice. Try again.\n");
    }

    queue[rear++] = start;
    visited[start] = 1;

    printf("\n  BFS Visit Order:\n  ");

    while (front < rear) {
        u = queue[front++];

        printf("%s", city[u].name);

        if (front < rear)
            printf(" -> ");

        for (v = 0; v < nodeCount; v++) {
            if (u != v &&
                graph[u][v] != INF &&
                !visited[v]) {

                visited[v] = 1;
                queue[rear++] = v;
            }
        }
    }

    printf("\n\n  Data Structure: Queue\n");
    printf("  Complexity: O(V^2) using adjacency matrix\n");
}

/* ========================= DIJKSTRA ========================= */

int minimumDistance(int dist[], int used[]) {
    int i;
    int best = INF;
    int index = -1;

    for (i = 0; i < nodeCount; i++) {
        if (!used[i] && dist[i] < best) {
            best = dist[i];
            index = i;
        }
    }

    return index;
}

void printRoute(int parent[], int node) {
    if (parent[node] == -1) {
        printf("%s", city[node].name);
        return;
    }

    printRoute(parent, parent[node]);
    printf(" -> %s", city[node].name);
}

void dijkstra(void) {
    int source, destination;

    int dist[MAX_NODES];
    int used[MAX_NODES] = {0};
    int parent[MAX_NODES];

    int i, step, u;

    title("FASTEST ROUTE - DIJKSTRA");

    printf("\n  Available intersections:\n");

    for (i = 0; i < nodeCount; i++)
        printf("  [%d] %s\n", i + 1, city[i].name);

    while (1) {
        char buffer[50];
        int choice;

        printf("\n  Source (1-%d): ", nodeCount);

        if (fgets(buffer, sizeof(buffer), stdin) &&
            sscanf(buffer, "%d", &choice) == 1 &&
            choice >= 1 && choice <= nodeCount) {
            source = choice - 1;
            break;
        }

        printf("  Invalid choice. Try again.\n");
    }

    while (1) {
        char buffer[50];
        int choice;

        printf("  Destination (1-%d): ", nodeCount);

        if (fgets(buffer, sizeof(buffer), stdin) &&
            sscanf(buffer, "%d", &choice) == 1 &&
            choice >= 1 && choice <= nodeCount) {
            destination = choice - 1;
            break;
        }

        printf("  Invalid choice. Try again.\n");
    }

    for (i = 0; i < nodeCount; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    dist[source] = 0;

    for (step = 0; step < nodeCount - 1; step++) {
        u = minimumDistance(dist, used);

        if (u == -1)
            break;

        used[u] = 1;

        for (i = 0; i < nodeCount; i++) {
            if (!used[i] &&
                graph[u][i] != INF &&
                dist[u] != INF &&
                dist[u] + graph[u][i] < dist[i]) {

                dist[i] = dist[u] + graph[u][i];
                parent[i] = u;
            }
        }
    }

    if (dist[destination] == INF) {
        printf("\n  No route exists between these intersections.\n");
        return;
    }

    printf("\n  FASTEST ROUTE\n");
    printf("  ");
    printRoute(parent, destination);

    printf("\n\n  Minimum travel time: %d minutes\n", dist[destination]);
    printf("  Algorithm: Dijkstra\n");
    printf("  Complexity: O(V^2)\n");
}

/* ========================= PRIM'S MST ========================= */

void primMST(void) {
    int selected[MAX_NODES] = {0};
    int key[MAX_NODES];
    int parent[MAX_NODES];

    int i, j, u;
    int totalWeight = 0;

    title("MINIMUM ROAD NETWORK - PRIM");

    for (i = 0; i < nodeCount; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    key[0] = 0;

    for (i = 0; i < nodeCount - 1; i++) {
        int min = INF;

        u = -1;

        for (j = 0; j < nodeCount; j++) {
            if (!selected[j] && key[j] < min) {
                min = key[j];
                u = j;
            }
        }

        if (u == -1)
            break;

        selected[u] = 1;

        for (j = 0; j < nodeCount; j++) {
            if (!selected[j] && graph[u][j] < key[j]) {
                key[j] = graph[u][j];
                parent[j] = u;
            }
        }
    }

    printf("\n  Selected roads:\n");
    line('-', 58);

    for (i = 1; i < nodeCount; i++) {
        if (parent[i] == -1) {
            printf("  Network is disconnected.\n");
            return;
        }

        printf("  %-22s -- %-22s : %d min\n",
               city[parent[i]].name,
               city[i].name,
               graph[parent[i]][i]);

        totalWeight += graph[parent[i]][i];
    }

    printf("\n  Minimum total road weight: %d minutes\n", totalWeight);
    printf("  Algorithm: Prim's Minimum Spanning Tree\n");
    printf("  Complexity: O(V^2)\n");
}

/* ========================= DISJOINT SET / UNION-FIND ========================= */

int findSet(int parent[], int x) {
    if (parent[x] == x)
        return x;

    parent[x] = findSet(parent, parent[x]);

    return parent[x];
}

void unionSet(int parent[], int rank[], int a, int b) {
    int rootA = findSet(parent, a);
    int rootB = findSet(parent, b);

    if (rootA == rootB)
        return;

    if (rank[rootA] < rank[rootB])
        parent[rootA] = rootB;
    else if (rank[rootA] > rank[rootB])
        parent[rootB] = rootA;
    else {
        parent[rootB] = rootA;
        rank[rootA]++;
    }
}

/* ========================= KRUSKAL'S MST ========================= */

void sortEdges(Edge edges[], int edgeCount) {
    int i, j;
    Edge key;

    /*
       Small graph: insertion sort keeps the implementation
       simple and easy to explain.
    */

    for (i = 1; i < edgeCount; i++) {
        key = edges[i];
        j = i - 1;

        while (j >= 0 && edges[j].weight > key.weight) {
            edges[j + 1] = edges[j];
            j--;
        }

        edges[j + 1] = key;
    }
}

void kruskalMST(void) {
    Edge edges[MAX_NODES * MAX_NODES];

    int edgeCount = 0;
    int parent[MAX_NODES];
    int rank[MAX_NODES] = {0};

    int i, j;
    int selected = 0;
    int totalWeight = 0;

    title("MINIMUM ROAD NETWORK - KRUSKAL");

    /*
       Convert adjacency matrix into an edge list.
    */

    for (i = 0; i < nodeCount; i++) {
        for (j = i + 1; j < nodeCount; j++) {
            if (graph[i][j] != INF) {
                edges[edgeCount].u = i;
                edges[edgeCount].v = j;
                edges[edgeCount].weight = graph[i][j];

                edgeCount++;
            }
        }
    }

    sortEdges(edges, edgeCount);

    for (i = 0; i < nodeCount; i++)
        parent[i] = i;

    printf("\n  Selected roads:\n");
    line('-', 58);

    for (i = 0;
         i < edgeCount && selected < nodeCount - 1;
         i++) {

        int a = edges[i].u;
        int b = edges[i].v;

        if (findSet(parent, a) != findSet(parent, b)) {

            unionSet(parent, rank, a, b);

            printf("  %-22s -- %-22s : %d min\n",
                   city[a].name,
                   city[b].name,
                   edges[i].weight);

            totalWeight += edges[i].weight;
            selected++;
        }
    }

    if (selected != nodeCount - 1) {
        printf("\n  Network is disconnected.\n");
        return;
    }

    printf("\n  Minimum total road weight: %d minutes\n", totalWeight);
    printf("  Algorithm: Kruskal's Minimum Spanning Tree\n");
    printf("  Data Structure: Disjoint Set / Union-Find\n");
    printf("  Complexity: O(E log E)\n");
}

/* ========================= ALGORITHM MENU ========================= */

void menu(void) {
    printf("\n");
    line('=', 68);

    printf("              SMART CITY TRAFFIC OPTIMIZER\n");

    line('=', 68);

    printf("  Intersections: %d\n\n", nodeCount);

    printf("  [1] Traffic & Signal Dashboard\n");
    printf("  [2] Traffic Ranking       -> Merge Sort\n");
    printf("  [3] Network Exploration   -> BFS\n");
    printf("  [4] Fastest Route         -> Dijkstra\n");
    printf("  [5] Minimum Road Network  -> Prim\n");
    printf("  [6] Minimum Road Network  -> Kruskal\n");
    printf("  [0] Exit\n");
}

/* ========================= MAIN ========================= */

int main(void) {
    int choice;
    char buffer[50];

    initializeGraph();
    loadSampleCity();

    printf("\n");
    line('=', 68);

    printf("       WELCOME TO SMART CITY TRAFFIC OPTIMIZER\n");

    line('=', 68);

    printf("\n  A sample city is already loaded.\n");
    printf("  Select any algorithm from the menu to explore the city.\n");

    while (1) {
        menu();

        printf("\n  Enter choice: ");

        if (!fgets(buffer, sizeof(buffer), stdin))
            break;

        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("  Invalid input. Please enter a menu number.\n");
            continue;
        }

        switch (choice) {

            case 1:
                showDashboard();
                break;

            case 2:
                rankTraffic();
                break;

            case 3:
                bfs();
                break;

            case 4:
                dijkstra();
                break;

            case 5:
                primMST();
                break;

            case 6:
                kruskalMST();
                break;

            case 0:
                printf("\n  Thank you for using Smart City Traffic Optimizer!\n");
                return 0;

            default:
                printf("  Invalid choice. Please select 0-6.\n");
        }

        pauseScreen();
    }

    return 0;
}
