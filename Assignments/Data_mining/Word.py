import codecs
import re

document = codecs.open('KCC150_Korean_sentences_UTF8.txt', 'r', "utf-8-sig")
# document = codecs.open('test.txt', 'r', "utf-8-sig") #\ufeff 제거
document_text = document.read()
# 한글자씩 : read()  |  단위줄 : readlines()

# print(ord(document_text[1]))
pattern = re.compile('[^ |\t|\n|가-힣]+')
document_split = pattern.sub('', document_text)
document_split = document_split.split()
# print(document_split)

frequency = {}
total = 0
for word in document_split:
    count = frequency.get(word, 0)
    frequency[word] = count + 1
    total += 1

result = sorted(frequency.items(), key=lambda frequency:frequency[1], reverse= True)
# print(result)

print()
print("        빈도,  출현확률,  누적출현확률")

context = {}
def percentage(num):
    accumulate = 0
    for n in range(len(result)):
        percent = (result[n][1] / total)
        accumulate += percent
        if(num):
            print("\'", result[n][0], "\' ", result[n][1], ", ", (round(percent, 2)*100), '%', ", ",
              (round(accumulate, 2)*100), '%')  # 반올림
            num -= 1
        context[result[n][0]] = percent

percentage(5)
print("총 음절 개수 : ", total, " 사용된 음절 개수 : ", len(result))
# print(context)

def unigram(sentence):
    sentence_split = pattern.sub('', sentence)
    sentence_split = sentence_split.split()

    sum = 1
    for word in sentence_split:
        if word in context:
            sum *= context[word]

    print(sentence_split, sum)

unigram('나는 밥을 먹고 학교에 간다')
unigram('오늘 학교가 끝났다')
unigram('고양이는 귀여워')
unigram('너무 아픈 사랑은 사랑이 아니었음을')
unigram('곧 종강할 수 있다')
unigram('크리스마스 기다려진다')
unigram('곧 내 생일이야')
unigram('생일 선물 뭐 줄거야')
unigram('너의 선물 기대해')

