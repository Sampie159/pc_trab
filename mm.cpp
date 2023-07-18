#include <benchmark/benchmark.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int inicio;
  int final;
} arg_t;

int matriz1_500[500][500];
int matriz2_500[500][500];
int matriz_res_seq1[500][500];
int matriz_res_par1[500][500];

int matriz1_1000[1000][1000];
int matriz2_1000[1000][1000];
int matriz_res_seq2[1000][1000];
int matriz_res_par2[1000][1000];

int matriz1_2000[2000][2000];
int matriz2_2000[2000][2000];
int matriz_res_seq3[2000][2000];
int matriz_res_par3[2000][2000];

static void DoSetup(const ::benchmark::State &state) {
  srand(1337);
  for (int i = 0; i < 500; i++) {
    for (int j = 0; j < 500; j++) {
      matriz1_500[i][j] = rand() % 100;
      matriz2_500[i][j] = rand() % 100;
    }
  }

  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      matriz1_1000[i][j] = rand() % 100;
      matriz2_1000[i][j] = rand() % 100;
    }
  }

  for (int i = 0; i < 2000; i++) {
    for (int j = 0; j < 2000; j++) {
      matriz1_2000[i][j] = rand() % 100;
      matriz2_2000[i][j] = rand() % 100;
    }
  }
}

static void *mm_par_500(void *arg) {
  arg_t *args = (arg_t *)arg;

  for (int i = args->inicio; i < args->final; i++) {
    for (int j = 0; j < 500; j++) {
      matriz_res_par1[i][j] = 0;
      for (int k = 0; k < 500; k++) {
        matriz_res_par1[i][j] += matriz1_500[i][k] * matriz2_500[k][j];
      }
    }
  }

  free(args);

  pthread_exit(NULL);
}

static void *mm_par_1000(void *arg) {
  arg_t *args = (arg_t *)arg;

  for (int i = args->inicio; i < args->final; i++) {
    for (int j = 0; j < 1000; j++) {
      matriz_res_par2[i][j] = 0;
      for (int k = 0; k < 1000; k++) {
        matriz_res_par2[i][j] += matriz1_1000[i][k] * matriz2_1000[k][j];
      }
    }
  }

  free(args);

  pthread_exit(NULL);
}

static void *mm_par_2000(void *arg) {
  arg_t *args = (arg_t *)arg;

  for (int i = args->inicio; i < args->final; i++) {
    for (int j = 0; j < 2000; j++) {
      matriz_res_par3[i][j] = 0;
      for (int k = 0; k < 2000; k++) {
        matriz_res_par3[i][j] += matriz1_2000[i][k] * matriz2_2000[k][j];
      }
    }
  }

  free(args);

  pthread_exit(NULL);
}

static void bm_seq_500(benchmark::State &state) {
  for (auto _ : state) {
    for (int i = 0; i < 500; i++) {
      for (int j = 0; j < 500; j++) {
        matriz_res_seq1[i][j] = 0;
        for (int k = 0; k < 500; k++) {
          matriz_res_seq1[i][j] += matriz1_500[i][k] * matriz2_500[k][j];
        }
      }
    }
  }
}
BENCHMARK(bm_seq_500)
    ->Setup(DoSetup)
    ->Iterations(10)
    ->Unit(benchmark::kSecond)
    ->UseRealTime();

static void bm_seq_1000(benchmark::State &state) {
  for (auto _ : state) {
    for (int i = 0; i < 1000; i++) {
      for (int j = 0; j < 1000; j++) {
        matriz_res_seq2[i][j] = 0;
        for (int k = 0; k < 1000; k++) {
          matriz_res_seq2[i][j] += matriz1_1000[i][k] * matriz2_1000[k][j];
        }
      }
    }
  }
}
BENCHMARK(bm_seq_1000)
    ->Setup(DoSetup)
    ->Iterations(10)
    ->Unit(benchmark::kSecond)
    ->UseRealTime();

static void bm_seq_2000(benchmark::State &state) {
  for (auto _ : state) {
    for (int i = 0; i < 2000; i++) {
      for (int j = 0; j < 2000; j++) {
        matriz_res_seq3[i][j] = 0;
        for (int k = 0; k < 2000; k++) {
          matriz_res_seq3[i][j] += matriz1_2000[i][k] * matriz2_2000[k][j];
        }
      }
    }
  }
}
BENCHMARK(bm_seq_2000)
    ->Setup(DoSetup)
    ->Iterations(10)
    ->Unit(benchmark::kSecond)
    ->UseRealTime();

static void bm_par_500(benchmark::State &state) {
#pragma omp parallel for
  for (auto _ : state) {
    int n_threads = state.range(0);
    pthread_t threads[n_threads];
    int qtd = 500 / n_threads;

    for (int i = 0; i < n_threads; i++) {
      arg_t *args = (arg_t *)malloc(sizeof(arg_t));
      args->inicio = i * qtd;
      args->final = i == n_threads - 1 ? 500 : (i + 1) * qtd;

      pthread_create(&threads[i], NULL, mm_par_500, (void *)args);
    }

    for (int i = 0; i < n_threads; i++) {
      pthread_join(threads[i], NULL);
    }
  }
}
BENCHMARK(bm_par_500)
    ->Arg(2)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Setup(DoSetup)
    ->Iterations(10)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->UseRealTime();

static void bm_par_1000(benchmark::State &state) {
#pragma omp parallel for
  for (auto _ : state) {
    int n_threads = state.range(0);
    pthread_t threads[n_threads];
    int qtd = 1000 / n_threads;

    for (int i = 0; i < n_threads; i++) {
      arg_t *args = (arg_t *)malloc(sizeof(arg_t));
      args->inicio = i * qtd;
      args->final = i == n_threads - 1 ? 1000 : (i + 1) * qtd;

      pthread_create(&threads[i], NULL, mm_par_1000, (void *)args);
    }

    for (int i = 0; i < n_threads; i++) {
      pthread_join(threads[i], NULL);
    }
  }
}
BENCHMARK(bm_par_1000)
    ->Arg(2)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Setup(DoSetup)
    ->Iterations(10)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->UseRealTime();

static void bm_par_2000(benchmark::State &state) {
#pragma omp parallel for
  for (auto _ : state) {
    int n_threads = state.range(0);
    pthread_t threads[n_threads];
    int qtd = 2000 / n_threads;

    for (int i = 0; i < n_threads; i++) {
      arg_t *args = (arg_t *)malloc(sizeof(arg_t));
      args->inicio = i * qtd;
      args->final = i == n_threads - 1 ? 2000 : (i + 1) * qtd;

      pthread_create(&threads[i], NULL, mm_par_2000, (void *)args);
    }

    for (int i = 0; i < n_threads; i++) {
      pthread_join(threads[i], NULL);
    }
  }
}
BENCHMARK(bm_par_2000)
    ->Arg(2)
    ->Arg(4)
    ->Arg(8)
    ->Arg(16)
    ->Setup(DoSetup)
    ->Iterations(10)
    ->Unit(benchmark::kSecond)
    ->MeasureProcessCPUTime()
    ->UseRealTime();

BENCHMARK_MAIN();
