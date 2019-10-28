import codecs

document = codecs.open('KCC150_Korean_sentences_UTF8.txt', 'r', "utf-8-sig")
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

context = {}
def percentage(num):
    accumulate = 0
    for n in range(len(result)):
        percent = (result[n][1] / total) * 100
        accumulate += percent
        if(num):
            print("\'", result[n][0], "\' ", result[n][1], ", ", round(percent, 2), '%', ", ", round(accumulate, 2), '%')  # 반올림
            num -= 1
        context[result[n][0]] = percent

percentage(5)

print("총 음절 개수 : ", total, " 사용된 음절 개수 : ", len(result))
print()

def unigram(sentence):
    sum = 1
    sentence_list = []
    for word in sentence:
        if word in context:
            sentence_list += word
            sum *= context[word]

    print(sentence_list, sum)

unigram('안녕하세요')
unigram('나는 밥을 먹고 학교에 간다')
unigram('오늘 학교가 끝났다')
unigram('고양이는 귀여워')
unigram('너무 아픈 사랑은 사랑이 아니었음을')
unigram('곧 종강할 수 있다')
unigram('크리스마스 기다려진다')
unigram('곧 내 생일이야')
unigram('생일 선물 뭐 줄거야')
unigram('너의 선물 기대해')