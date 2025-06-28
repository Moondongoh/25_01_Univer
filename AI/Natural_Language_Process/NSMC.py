import torch
from ratsnlp.nlpbook.classification import ClassificationTrainArguments
from ratsnlp import nlpbook
from Korpora import Korpora
from transformers import BertTokenizer
from torch.utils.data import DataLoader, SequentialSampler, RandomSampler
from ratsnlp.nlpbook.classification import NsmcCorpus, ClassificationDataset
from transformers import BertConfig, BertForSequenceClassification
from ratsnlp.nlpbook.classification import ClassificationTask
import os

def main():
    # 로컬 경로 설정
    BASE_DIR = "D:/MDO/AI_LEE/check-point"
    os.makedirs(BASE_DIR, exist_ok=True)

    args = ClassificationTrainArguments(
        pretrained_model_name="beomi/kcbert-base",
        downstream_corpus_name="nsmc",
        downstream_corpus_root_dir=BASE_DIR,
        downstream_model_dir=os.path.join(BASE_DIR, "checkpoint-doccls"),
        batch_size=32 if torch.cuda.is_available() else 4,
        learning_rate=5e-5,
        max_seq_length=128,
        epochs=3,
        tpu_cores=0,
        seed=7,
        cpu_workers=4  # 오류 피하려면 일단 4 이하로 줄여도 좋음
    )

    # 설정
    nlpbook.set_seed(args)
    nlpbook.set_logger(args)

    # 데이터 다운로드
    Korpora.fetch(
        corpus_name=args.downstream_corpus_name,
        root_dir=args.downstream_corpus_root_dir,
        force_download=True,
    )

    # 토크나이저 로딩
    tokenizer = BertTokenizer.from_pretrained(
        args.pretrained_model_name,
        do_lower_case=False,
    )

    # 학습 데이터셋 준비
    corpus = NsmcCorpus()
    train_dataset = ClassificationDataset(
        args=args,
        corpus=corpus,
        tokenizer=tokenizer,
        mode="train",
    )
    train_dataloader = DataLoader(
        train_dataset,
        batch_size=args.batch_size,
        sampler=RandomSampler(train_dataset, replacement=False),
        collate_fn=nlpbook.data_collator,
        drop_last=False,
        num_workers=args.cpu_workers,
    )

    # 검증 데이터셋 준비
    val_dataset = ClassificationDataset(
        args=args,
        corpus=corpus,
        tokenizer=tokenizer,
        mode="test",
    )
    val_dataloader = DataLoader(
        val_dataset,
        batch_size=args.batch_size,
        sampler=SequentialSampler(val_dataset),
        collate_fn=nlpbook.data_collator,
        drop_last=False,
        num_workers=args.cpu_workers,
    )

    # 모델 구성
    pretrained_model_config = BertConfig.from_pretrained(
        args.pretrained_model_name,
        num_labels=corpus.num_labels,
    )
    model = BertForSequenceClassification.from_pretrained(
        args.pretrained_model_name,
        config=pretrained_model_config,
    )

    # 학습 태스크 및 트레이너 설정
    task = ClassificationTask(model, args)
    trainer = nlpbook.get_trainer(args)

    # 모델 학습
    trainer.fit(
        task,
        train_dataloaders=train_dataloader,
        val_dataloaders=val_dataloader,
    )

if __name__ == "__main__":
    main()
