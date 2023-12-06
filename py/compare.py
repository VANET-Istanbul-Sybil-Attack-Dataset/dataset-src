import os, glob, json
from pprint import pprint

BSM_COMPARE_FIEDS = ["senderPseudo", "messageID", "pos", "pos_noise", "spd", "spd_noise", "acl", "acl_noise", "hed", "hed_noise"]


def read_json(f):
    l = []
    for line in open(f).readlines():
        if line.strip() == '':
            continue
        data = json.loads(line)
        l.append(data)
    return l

def read_txt(f):
    main_fields = ["type", "recv", "bsm", "label"]

    bsm_fields = ["time", "vehicleId", "messageID", "gps",
                  "pos", "pos_noise", "spd", "spd_noise", "hed", "hed_noise",
                  "acl", "acl_noise", "width", "length"]
    recv_fields = ["rssi"] + bsm_fields
    label_fields = ["attackType", "powerControlType", "txPower"] + bsm_fields

    receiver = f.split("/")[-1].split("-")[1] if not "traceGroundTruth" in f else None

    l = []
    for line in open(f).readlines():
        line = line.strip()
        if line == '':
            continue
        x = {k:v for k,v in zip(main_fields, line.split("#"))}
        x["type"] = int(x["type"])
        if x["recv"] != "":
            x["recv"] = {k:v for k,v in zip(recv_fields, x["recv"].split("|"))}
            # print(x["recv"]["vehicleId"], receiver)
            assert receiver == None or x["recv"]["vehicleId"] == receiver
        else:
            x["recv"] = None
        if x["bsm"] != "":
            x["bsm"] = {k:v for k,v in zip(bsm_fields, x["bsm"].split("|"))}
        else:
            x["bsm"] = None
        if x["label"] != "":
            x["label"] = {k:v for k,v in zip(label_fields, x["label"].split("|"))}
        else:
            x["label"] = None

        # values = line.split("|")
        # assert(len(values) == len(fields))
        # l.append({k:v for k,v in zip(fields, values)})
        l.append(x)
    return l




def check_not_contains_duplicate_id(a):
    a = [x["bsm"]["messageID"] for x in a if x["type"] in [3, 4]]
    assert(len(a) == len(set(a)))



def compare_messages(a, gt):
    check_not_contains_duplicate_id(a)
    check_not_contains_duplicate_id(gt)

    dict_a = {x["bsm"]["messageID"]:x for x in a if x["type"] in [3, 4]}
    dict_gt = {x["bsm"]["messageID"]:x for x in gt if x["type"] in [3, 4]}

    for id, data_a in dict_a.items():
        data_gt = dict_gt[id]
        # print()
        # print("ground truth:")
        # pprint(data_gt)
        # print("data:")
        # pprint(data_a)
        # eq = sorted(set([data_a["bsm"][k] == data_gt["bsm"][k] for k in BSM_COMPARE_FIEDS]))
        # if eq == [True]:
        #     assert(data_gt["label"]["attackType"] in ['0', '22'])
        # else:
        #     assert(data_gt["label"]["attackType"] != '0')

    print("OK")


def print_messages(a, gt, sender, sort=None):
    dict_a = {x["bsm"]["messageID"]:x for x in a if x["type"] in [0, 1]}
    dict_gt = {x["bsm"]["messageID"]:x for x in gt if x["type"] in [0, 1]}

    if sort == "sendTime":
        items = sorted(dict_a.items(), key=lambda x:float(x[1]["bsm"]["sendTime"]))
    else:
        items = dict_a.items()

    for id, data_a in items:
        data_gt = dict_gt[id]
        # if data_gt["label"]["sender"] != sender:
        #     continue
        print()
        print("ground truth:")
        pprint(data_gt)
        print("data:")
        pprint(data_a)

    print("OK")


def print_ground_truth(gt):
    for data in gt:
        print()
        pprint(data)

def print_data(a):
    for data in a:
        print()
        pprint(data)

def main():
    assert(len(os.listdir("/tmp/veins-results/veins-example")) == 1)
    # ground_truth_path = glob.glob("/tmp/veins-results/veins-example/*/traceGroundTruthJSON-0.json")[0]
    # a_path = glob.glob("/tmp/veins-results/veins-example/*/traceJSON-28-26-A0-10-0.json")[0]
    # ground_truth = read_json(ground_truth_path)
    # a = read_json(a_path)

    ground_truth_path = glob.glob("/tmp/veins-results/veins-example/*/traceGroundTruth-0.txt")[0]
    a_path = glob.glob("/tmp/veins-results/veins-example/*/trace-*-A0-*.txt")[0]
    ground_truth = read_txt(ground_truth_path)
    a = read_txt(a_path)
    # pprint(a)
    compare_messages(a, ground_truth)

    print_messages(a, ground_truth, '34')
    # print_messages(a, ground_truth, '22', sort="sendTime")

    # print_ground_truth(ground_truth)
    # print_data(a)


if __name__ == "__main__":
    main()
