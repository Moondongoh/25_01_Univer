# 라이브러리 포함
import FinanceDataReader as fdr
import numpy as np
import matplotlib.pyplot as plt

# 삼성전자 코드='005930', 2020년 데이터부터 다운로드
samsung = fdr.DataReader('005930', '2024')

# 시작가만 취한다.
seq_data = (samsung[['Open']]).to_numpy()
# 선형 그래프로 그린다.
plt.plot(seq_data, color='blue')
plt.title("Samsung Electronics Stock Price")
plt.xlabel("days")
plt.ylabel("price")
plt.show()

seq_data = (samsung[['Open']]).to_numpy()

def make_sample(data, window):
    train = []					# 공백 리스트 생성
    target = []
    for i in range(len(data)-window):		# 데이터의 길이만큼 반복
        train.append(data[i:i+window])		# i부터 (i+window-1) 까지를 저장
        target.append(data[i+window])		# (i+window) 번째 요소는 정답
    return np.array(train), np.array(target)	# 훈련 샘플과 정답 레이블을 반환

X, y = make_sample(seq_data, 7)		# 윈도우 크기=7
print(X.shape, y.shape)			# 넘파이 배열의 형상 출력
print(X[0], y[0])				# 첫 번째 샘플 출력