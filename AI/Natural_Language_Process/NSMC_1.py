import os
import csv
import torch
from torch.utils.data import DataLoader, SequentialSampler, RandomSampler
from transformers import BertTokenizer, BertConfig, BertForSequenceClassification
from ratsnlp import nlpbook
from ratsnlp.nlpbook.classification import (
    ClassificationTrainArguments,
    ClassificationDataset,
    ClassificationTask,
)
from ratsnlp.nlpbook.classification.corpus import ClassificationExample


class NsmcCorpus2:
    def __init__(self):
        pass

    def get_examples(self, data_root_path, mode):
        data_fpath = os.path.join(data_root_path, f"ratings_{mode}.txt")
        lines = list(csv.reader(open(data_fpath, "r", encoding="utf-8"), delimiter="\t", quotechar='"'))
        examples = []
        for i, line in enumerate(lines):
            if i == 0:
                continue
            _, text_a, label = line
            examples.append(ClassificationExample(text_a=text_a, text_b=None, label=label))
        return examples

    def get_labels(self):
        return ["0", "1"]

    @property
    def num_labels(self):
        return len(self.get_labels())


def main():
    # Windows에서는 절대경로를 지정해 주세요
    base_dir = "D:/MDO/AI_LEE/check-point"

    args = ClassificationTrainArguments(
        pretrained_model_name="beomi/kcbert-base",
        downstream_corpus_name="nsmc",
        downstream_corpus_root_dir=base_dir,
        downstream_model_dir=os.path.join(base_dir, "checkpoint-doccls"),
        batch_size=32 if torch.cuda.is_available() else 4,
        learning_rate=5e-5,
        max_seq_length=128,
        epochs=5,
        tpu_cores=0,  # Windows에는 TPU 없음
        seed=7,
        cpu_workers=4,  # 0~4 권장
    )

    nlpbook.set_seed(args)
    nlpbook.set_logger(args)

    # 데이터가 이미 다운로드되어 있다고 가정하거나 다운로드 경로 직접 제공
    corpus = NsmcCorpus2()

    tokenizer = BertTokenizer.from_pretrained(
        args.pretrained_model_name,
        do_lower_case=False,
    )

    train_dataset = ClassificationDataset(
        args=args,
        corpus=corpus,
        tokenizer=tokenizer,
        mode="train",
    )

    train_dataloader = DataLoader(
        train_dataset,
        batch_size=args.batch_size,
        sampler=RandomSampler(train_dataset),
        collate_fn=nlpbook.data_collator,
        drop_last=False,
        num_workers=args.cpu_workers,
    )

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

    pretrained_model_config = BertConfig.from_pretrained(
        args.pretrained_model_name,
        num_labels=corpus.num_labels,
    )

    model = BertForSequenceClassification.from_pretrained(
        args.pretrained_model_name,
        config=pretrained_model_config,
    )

    task = ClassificationTask(model, args)
    trainer = nlpbook.get_trainer(args)

    trainer.fit(
        task,
        train_dataloaders=train_dataloader,
        val_dataloaders=val_dataloader,
    )


if __name__ == "__main__":
    import multiprocessing
    multiprocessing.freeze_support()  # 안전하게 멀티프로세싱을 위해 추가
    main()
