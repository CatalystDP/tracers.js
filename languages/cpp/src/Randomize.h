#ifndef PROJECT_RANDOMIZE_H
#define PROJECT_RANDOMIZE_H

#include <random>
#include <algorithm>

namespace Randomize {
    using std::string;
    using std::mt19937;

    template<class T>
    class Randomizer {
    public:
        Randomizer() {
        }

        virtual T create() {
        }
    };


    class Integer : public Randomizer<int> {
    private:
        int _min;
        int _max;

        mt19937 rng;

    public:
        Integer(int min = 1, int max = 9) {
            _min = min;
            _max = max;

            rng.seed(std::random_device()());
        }

        int create() {
            std::uniform_int_distribution<int> dist6(_min, _max);
            return dist6(rng);
        }
    };


    class Double : public Randomizer<double> {
    private:
        double _min;
        double _max;

        mt19937 rng;

    public:
        Double(double min = 0, double max = 1) {
            _min = min;
            _max = max;

            rng.seed(std::random_device()());
        }

        double create() {
            std::uniform_real_distribution<double> dist6(_min, _max);
            return dist6(rng);
        }
    };


    class String : public Randomizer<string> {
    private:
        int _length;
        string _letters;

    public:
        String(int length = 16, string letters = "abcdefghijklmnopqrstuvwxyz") {
            _length = length;
            _letters = letters;
        }

        string create() {
            string text = "";
            Integer randomizer(0, _letters.length() - 1);
            for (int i = 0; i < _length; i++) {
                text += _letters[randomizer.create()];
            }
            return text;
        }
    };


    template<class T>
    class Array2D : public Randomizer<T **> {
    private:
        int _N;
        int _M;
        Randomizer<T> *_randomizer;
        bool _sorted;

    public:
        Array2D(int N = 10, int M = 10, Randomizer<T> &randomizer = *(new Integer())) {
            _N = N;
            _M = M;
            _randomizer = &randomizer;
            _sorted = false;
        }

        Array2D sorted(bool sorted = true) {
            _sorted = sorted;
            return *this;
        }

        T **create() {
            T **D = new T *[_N];
            for (int i = 0; i < _N; i++) {
                D[i] = new T[_M];
                for (int j = 0; j < _M; j++) {
                    D[i][j] = (*_randomizer).create();
                }
                if (_sorted) std::sort(D[i], D[i] + _M);
            }
            return D;
        }
    };


    template<class T>
    class Array1D : public Randomizer<T *> {
    private:
        int _N;
        Randomizer<T> *_randomizer;
        bool _sorted;

    public:
        Array1D(int N = 10, Randomizer<T> &randomizer = *(new Integer())) {
            _N = N;
            _randomizer = &randomizer;
            _sorted = false;
        }

        Array1D sorted(bool sorted = true) {
            _sorted = sorted;
            return *this;
        }

        // TODO: fix array1d/2d/graph randomizers returning pointer instead of array
        T *create() {
            T *D = new T[_N];
            for (int i = 0; i < _N; i++) {
                D[i] = (*_randomizer).create();
            }
            if (_sorted) std::sort(D, D + _N);
            return D;
        }
    };


    template<class T>
    class Graph : public Randomizer<T **> {
    private:
        int _N;
        double _ratio;
        Randomizer<T> *_randomizer;
        bool _directed;
        bool _weighted;

    public:
        Graph(int N = 5, double ratio = .3, Randomizer<T> &randomizer = *(new Integer())) {
            _N = N;
            _ratio = ratio;
            _randomizer = &randomizer;
            _directed = true;
            _weighted = false;
        }

        Graph directed(bool directed = true) {
            _directed = directed;
            return *this;
        }

        Graph weighted(bool weighted = true) {
            _weighted = weighted;
            return *this;
        }

        T **create() {
            T **G = new T *[_N];
            for (int i = 0; i < _N; i++) {
                G[i] = new T[_N];
            }
            Double ratioRandomizer;
            for (int i = 0; i < _N; i++) {
                for (int j = 0; j < _N; j++) {
                    if (i == j) {
                        G[i][j] = 0;
                    } else if (_directed || i < j) {
                        G[i][j] = ratioRandomizer.create() < _ratio ? _weighted ? (*_randomizer).create() : 1 : 0;
                    } else {
                        G[i][j] = G[j][i];
                    }
                }
            }
            return G;
        }
    };
}

#endif //PROJECT_RANDOMIZE_H
