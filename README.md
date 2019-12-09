# CMake Build
프로젝트 루트에서 명령을 실행한다.
- **Debug** `mkdir -p Debug && cd Debug && cmake -DCMAKE_BUILD_TYPE=Debug ..`
- **Release** `mkdir -p Release && cd Release && cmake -DCMAKE_BUILD_TYPE=Release ..`

위의 명령을 실행한 다음 `Debug` 혹은 `Release` 디렉토리에 들어가서 `make`를 수행한다.

# Build
`make`

# Test
`make test`

# Run
`Release/src/a.out <1st arg> <2nd arg> <3th arg> <4th arg> <5th arg>`

## Arguments
- **1st arg** `bruteforce`, `kdtree` `rtree`
- **2nd arg** `range`, `kNN`
- **3rd arg** 좌표 목록이 기록된 파일의 경로를 지정함
- **4rd arg** 질의 파라미터들이 기록된 파일의 경로를 지정함
  - 질의 파라미터는 여러 개 기록할 수 있다.
- **5rd arg** 질의 연산 결과들을 기록한 파일을 저장할 경로를 지정한다.
  - 해당 파일에는 **4rd arg**에 기록된 파라미터의 개수만큼의 항목이 존재한다

## 좌표 목록 파일
다음과 같은 형식으로 저장돼있어야 한다
```
x1, y1
x2, y2
x3, y3
```

## 질의 파라미터 파일
다음과 같은 형식으로 저장돼있어야 한다 

range query 파라미터의 경우:
```
x1 y1 r1
x2 y2 r2
x3 y3 r3
```

위의 경우 실제 코드 상에서는 독립된 query 연산이 3번 실행된다.

kNN query 파라미터의 경우도 동일하다

## Standart Output
각 query 연산에 대한 실행시간(ms)과 검사한 객체 개수가 출력된다

예를 들어:
```
10.23 100
11.11 56
12.34 67
```

질의 파라미터 파일에 들어간 항목 개수만큼 출력된다. 

## Output Files
각 query 연산에 대한 결과 데이터(좌표 목록)가 파일에 기록된다.

질의 파라미터 파일의 있는 항목의 개수가 3개라면 실행 시 5번째 인자로 지정된 경로에:
> output_000.txt  
> output_001.txt  
> output_002.txt  

가 생성된다.

## 예시 입력 조합
```
Release/src/a.out bruteforce range /path/to/clustered_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out kdtree range /path/to/clustered_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out rtree range /path/to/clustered_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out bruteforce range /path/to/gaussian_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out kdtree range /path/to/gaussian_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out rtree range /path/to/gaussian_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out bruteforce range /path/to/uniformed_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out kdtree range /path/to/uniformed_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out rtree range /path/to/uniformed_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out bruteforce kNN /path/to/clustered_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out kdtree kNN /path/to/clustered_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out rtree kNN /path/to/clustered_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out bruteforce kNN /path/to/gaussian_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out kdtree kNN /path/to/gaussian_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out rtree kNN /path/to/gaussian_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out bruteforce kNN /path/to/uniformed_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out kdtree kNN /path/to/uniformed_dataset.txt /path/to/metric_out_dir /path/to/out_dir
Release/src/a.out rtree kNN /path/to/uniformed_dataset.txt /path/to/metric_out_dir /path/to/out_dir
```
