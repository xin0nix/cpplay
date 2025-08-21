import pytest


def generate_hashtag(raw_text: str):
    # your code here
    message = raw_text.strip()
    if len(message) == 0:
        return False
    words = map(lambda w: w.capitalize(), message.split())
    return "#" + "".join(words)


@pytest.mark.it("Generates correct hashtags for fixed input cases")
@pytest.mark.parametrize(
    "input_str, expected",
    [
        ("Codewars", "#Codewars"),
        ("Codewars      ", "#Codewars"),
        ("      Codewars", "#Codewars"),
        ("Codewars Is Nice", "#CodewarsIsNice"),
        ("codewars is nice", "#CodewarsIsNice"),
        ("CoDeWaRs is niCe", "#CodewarsIsNice"),
        ("c i n", "#CIN"),
        ("codewars  is  nice", "#CodewarsIsNice"),
        ("Hello there thanks for trying my Kata", "#HelloThereThanksForTryingMyKata"),
        ("    Hello     World   ", "#HelloWorld"),
        ("test", "#Test"),
        ("Test", "#Test"),
        ("    test    case   ", "#TestCase"),
        ("test123 case456", "#Test123Case456"),
        ("123 abc", "#123Abc"),
        ("code " * 10, "#CodeCodeCodeCodeCodeCodeCodeCodeCodeCode"),
        ("with PUNCTUATION, please!", "#WithPunctuation,Please!"),
        ("UPPER lower MiXeD", "#UpperLowerMixed"),
        ("one-word", "#One-word"),
        ("emoji 😊 are cool", "#Emoji😊AreCool"),
        ("titleCased Input", "#TitlecasedInput"),
        ("multiple    spaces here", "#MultipleSpacesHere"),
        ("a b c", "#ABC"),
        ("_", "#_"),
        ("___", "#___"),
        ("special characters !@#", "#SpecialCharacters!@#"),
    ],
)
def test_generate_hashtag(input_str, expected):
    assert generate_hashtag(input_str) == expected
