#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_linalg.h> // Include the necessary header file for the gsl library

// Define a structure to hold resistor details
typedef struct {
  char type;
  int* nodes;
  int node_count;
  double value;
} Component;

// Function prototypes
Component parse_line(char* line);
void solve_circuit(Component components[], int component_count, double *results);
void output_results(Component components[], double *results, int component_count);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <input_file.csv> \n", argv[0]);
    return 1;
  }

  Component components[50];  
  double results[50];        
  int component_count = 0;

  // Parse the CSV file and fill components array
  FILE *file = fopen(argv[1], "r");
  if (!file) {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }

  char buffer[256];
  fgets(buffer, 256, file);
  while (fgets(buffer, 256, file)) {
    if (buffer[0] == '#') continue; // Skip comments
    Component c = parse_line(buffer);
    components[component_count++] = c;

  }

  fclose(file);

  solve_circuit(components, component_count, results);
  output_results(components, results, component_count);

  return 0;
}

Component parse_line(char* line) {
  Component element = {0};
  char* token = strtok(line, ",");

  if (token == NULL) {
    printf("Error: Line does not contain a comma\n");
    return element;
  }

  element.type = token[0];

  // Ignore lines starting with '#'
  if (element.type == '#') {
    element.type = 0;
    return element;
  }

  // Handle 'L' type lines
  if (element.type == 'L') {
    element.node_count = 0;
    element.nodes = NULL;
    token = strtok(NULL, ",");
    while (token != NULL) {
      int* temp = realloc(element.nodes, (element.node_count + 1) * sizeof(int));
      if (temp == NULL) {
        printf("Error: Failed to allocate memory for nodes\n");
        free(element.nodes);
        return element;
      }
      element.nodes = temp;
      element.nodes[element.node_count] = atoi(token);
      element.node_count++;
      token = strtok(NULL, ",");
    }
    return element;
  }

  // Handle 'V' and 'R' type lines
  token = strtok(NULL, ",");
  if (token == NULL) {
    printf("Error: 'V' or 'R' type line does not contain enough tokens\n");
    return element;
  }
  element.node_count = 0;
  element.nodes = NULL;

  while (token != NULL) {
    int* temp = realloc(element.nodes, (element.node_count + 1) * sizeof(int));
    if (temp == NULL) {
      printf("Error: Failed to allocate memory for nodes\n");
      free(element.nodes);
      return element;
    }
    element.nodes = temp;
    element.nodes[element.node_count] = atoi(token);
    element.node_count++;
    token = strtok(NULL, ",");
  }

  // The last token is the value, not a node
  if (element.node_count > 0) {
    element.value = element.nodes[element.node_count - 1];
    element.node_count--;
  } else {
    // If there is no value, set it to 0
    element.value = 0;
  }  

  return element;
}

void solve_circuit(Component components[], int component_count, double *results) {
  int max_node = 0;
  for (int i = 0; i < component_count; i++) {
    if (components[i].type != 'L') {
      max_node = (max_node < components[i].nodes[0]) ? components[i].nodes[0] : max_node;
      max_node = (max_node < components[i].nodes[1]) ? components[i].nodes[1] : max_node;
    }
  }

  gsl_matrix *A = gsl_matrix_alloc(max_node, max_node);
  gsl_vector *b = gsl_vector_alloc(max_node);
  gsl_vector *x = gsl_vector_alloc(max_node);

  // Initialize A and b with zeros
  gsl_matrix_set_zero(A);
  gsl_vector_set_zero(b);

  // Fill A and b based on the circuit components
  for (int i = 0; i < component_count; i++) {
    if (components[i].type == 'R') {
      int node1 = components[i].nodes[0] - 1;
      int node2 = components[i].nodes[1] - 1;
      double value = components[i].value;

      gsl_matrix_set(A, node1, node1, gsl_matrix_get(A, node1, node1) + 1/value);
      gsl_matrix_set(A, node1, node2, gsl_matrix_get(A, node1, node2) - 1/value);
      gsl_matrix_set(A, node2, node1, gsl_matrix_get(A, node2, node1) - 1/value);
      gsl_matrix_set(A, node2, node2, gsl_matrix_get(A, node2, node2) + 1/value);
    }
    else if (components[i].type == 'V') {
      int node1 = components[i].nodes[0] - 1;
      int node2 = components[i].nodes[1] - 1;
      double value = components[i].value;

      gsl_vector_set(b, node1, value);
      gsl_vector_set(b, node2, -value);
    }
  }

  // Solve the linear system using GSL
  int s;
  gsl_permutation *p = gsl_permutation_alloc(max_node);
  gsl_linalg_LU_decomp(A, p, &s);
  gsl_linalg_LU_solve(A, p, b, x);

  // Store the results
  for (int i = 0; i < max_node; i++) {
    results[i] = gsl_vector_get(x, i);
  }

  gsl_permutation_free(p);
  gsl_vector_free(x);
  gsl_vector_free(b);
  gsl_matrix_free(A);
}


void output_results(Component components[], double *results, int component_count) {
  FILE *file = fopen("ans.csv", "w");
  if (!file) {
    perror("Failed to output results");
    exit(EXIT_FAILURE);
  }

  fprintf(file, "# Type | Node 1 | Node 2 | Value\n");
  for (int i = 0; i < component_count; i++) {
    if (components[i].type == 'R') {
      fprintf(file, "I,%d,%d,%.4f\n", components[i].nodes[0], components[i].nodes[1], results[i]);
    }
  }

  fclose(file);
}
