#include <cstdlib>
#include <gtest/gtest.h>

typedef struct {
  int inicio;
  int final;
} args_t;

static int matriz1[500][500];
static int matriz2[500][500];
static int matriz_res_seq[500][500];
static int matriz_res_par[500][500];

static void inicializar_matriz(void) {
  srand(1337);
  for (int i = 0; i < 500; i++) {
    for (int j = 0; j < 500; j++) {
      matriz1[i][j] = rand() % 500;
      matriz2[i][j] = rand() % 500;
    }
  }
}

static void mm_seq(void) {
  for (int i = 0; i < 500; i++) {
    for (int j = 0; j < 500; j++) {
      matriz_res_seq[i][j] = 0;
      for (int k = 0; k < 500; k++) {
        matriz_res_seq[i][j] += matriz1[i][k] * matriz2[k][j];
      }
    }
  }
}

static void *mm_par(void *arg) {
  args_t *args = (args_t *)arg;

  for (int i = args->inicio; i < args->final; i++) {
    for (int j = 0; j < 500; j++) {
      matriz_res_par[i][j] = 0;
      for (int k = 0; k < 500; k++) {
        matriz_res_par[i][j] += matriz1[i][k] * matriz2[k][j];
      }
    }
  }

  free(args);

  pthread_exit(NULL);
}

struct Teste : public ::testing::Test {
  void SetUp() override {
    inicializar_matriz();
    mm_seq();

    pthread_t threads[4];
    int qtd_linhas = 500 / 4;
    for (int i = 0; i < 4; i++) {
      args_t *args = (args_t *)malloc(sizeof(args_t));
      args->inicio = i * qtd_linhas;
      args->final = i == 3 ? 500 : (i + 1) * qtd_linhas;
      pthread_create(&threads[i], NULL, mm_par, (void *)args);
    }

    for (int i = 0; i < 4; i++) {
      pthread_join(threads[i], NULL);
    }
  }
};

TEST_F(Teste, igualdade) {
  ASSERT_EQ(0, memcmp(matriz_res_seq, matriz_res_par, sizeof(matriz_res_seq)));
}
