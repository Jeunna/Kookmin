# 가 AC00

# 초성 19개
FIRST = ['ㄱ', 'ㄲ', 'ㄴ', 'ㄷ', 'ㄸ', 'ㄹ', 'ㅁ', 'ㅂ', 'ㅃ', 'ㅅ', 'ㅆ', 'ㅇ', 'ㅈ', 'ㅉ', 'ㅊ', 'ㅋ', 'ㅌ', 'ㅍ', 'ㅎ']
# 중성 21개
MIDDEL = ['ㅏ', 'ㅐ', 'ㅑ', 'ㅒ', 'ㅓ', 'ㅔ', 'ㅕ', 'ㅖ', 'ㅗ', 'ㅘ', 'ㅙ', 'ㅚ', 'ㅛ', 'ㅜ', 'ㅝ', 'ㅞ', 'ㅟ', 'ㅠ', 'ㅡ', 'ㅢ', 'ㅣ']
# 종성 27+1개
LAST = ['', 'ㄱ', 'ㄲ', 'ㄳ', 'ㄴ', 'ㄵ', 'ㄶ', 'ㄷ', 'ㄹ', 'ㄺ', 'ㄻ', 'ㄼ', 'ㄽ', 'ㄾ', 'ㄿ', 'ㅀ', 'ㅁ', 'ㅂ', 'ㅄ', 'ㅅ', 'ㅆ', 'ㅇ', 'ㅈ', 'ㅊ', 'ㅋ', 'ㅌ', 'ㅍ', 'ㅎ']

def split(word):
    result = []
    for i in word:
        # ord : 문자의 아스키값 리턴
        # // : 몫  |   / : 나누기
        word = ord(word) - ord('가')
        first = word//(21*28)
        middle = (word - (first*21*28))//28
        last = word - first*21*28 - middle*28
    print(FIRST[first], MIDDEL[middle], LAST[last], ", ", first, middle, last)

def combine(first, middle, last):
    result = first*21*28 + middle*28 + last + 1
    result += ord('가')
    print(chr(result))

split("깐")
split('호')
split("끡")

combine(1, 0, 3)
combine(18, 8, -1)
combine(1, 19, 0)
