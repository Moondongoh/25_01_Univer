from transformers import BertModel, BertTokenizer

# KoBERT 로드
tokenizer = BertTokenizer.from_pretrained("monologg/kobert")
model = BertModel.from_pretrained("monologg/kobert")

print("KoBERT 로딩 성공!")
