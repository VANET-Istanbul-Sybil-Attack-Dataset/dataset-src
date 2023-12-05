import glob
import json
from pprint import pprint

def main():
    pseudonym_set = set()
    sender_id_set = set()

    sender_pseudo_map = dict()

    counts = {}
    for f in glob.glob("/tmp/veins-results/veins-example/*/traceJSON-*"):
        counts[f] = 0
        for l in open(f).readlines():
            j = json.loads(l)
            if j["type"] == 3:
                pseudonym_set.add(j["senderPseudo"])
                sender_id_set.add(j["sender"])
                sender_pseudo_map[j["sender"]] = sender_pseudo_map.get(j["sender"], set()) | set([j["senderPseudo"]])
                counts[f] += 1

    print("pseudonym_set", pseudonym_set)
    print("sender_id_set", sender_id_set)
    print(sender_pseudo_map)
    pprint(counts)

if __name__ == "__main__":
    main()
