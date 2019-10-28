import codecs

document = codecs.open('kowiki-20190901-pages-articles-multistream-index.txt', 'r', "utf-8-sig")
# document = codecs.open('test.txt', 'r', "utf-8-sig") #\ufeff 제거
document_text = document.read()
# 한글자씩 : read()  |  단위줄 : readlines()

# print(ord(document_text[1]))

frequency = {}
total = 0
for word in document_text:
    if ord('가') <= ord(word) and ord(word) <= ord('힣'):
        count = frequency.get(word, 0)
        frequency[word] = count + 1
        total += 1

result = sorted(frequency.items(), key=lambda frequency:frequency[1], reverse= True)
# result = frequency.keys()

# print("음절 TOP 5 : ")
# for i in range(5):
#     print(result[i])

print()
print("총 음절 개수 : ", total, " 사용된 음절 개수 : ", len(result))
print()
print("        빈도,  출현확률,  누적출현확률")

def percentage(num):
    accumulate = 0
    for n in range(num):
        percent = (result[n][1] / total) * 100
        accumulate += percent
        print("\'", result[n][0], "\' ", result[n][1], ", ", round(percent, 2), '%', ", ", round(accumulate, 2), '%')  # 반올림

percentage(2054)
print("총 음절 개수 : ", total, " 사용된 음절 개수 : ", len(result))