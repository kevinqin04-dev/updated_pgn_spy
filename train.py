import numpy as np
import xgboost as xgb
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, roc_auc_score

def load_move_times(filename):
    games = []
    with open(filename, "r") as f:
        for line in f:
            times = [int(x) for x in line.split() if x.strip()]
            if len(times) >= 5:
                games.append(times)
    return games

def extract_features(times):
    arr = np.array(times)
    mean = np.mean(arr)
    std = np.std(arr)
    median = np.median(arr)
    min_val = np.min(arr)
    max_val = np.max(arr)
    

    return [mean, std, median, min_val, max_val]



cheat_file = "cheat_movetimes.txt" 
clean_games = load_move_times("clean_movetimes.txt")
cheater_games = load_move_times(cheat_file)

X_clean = [extract_features(g) for g in clean_games]
y_clean = [0] * len(X_clean)

X_cheater = [extract_features(g) for g in cheater_games]
y_cheater = [1] * len(X_cheater)

X = np.array(X_clean + X_cheater)
y = np.array(y_clean + y_cheater)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

dtrain = xgb.DMatrix(X_train, label=y_train)
dtest = xgb.DMatrix(X_test, label=y_test)

params = {
    'objective': 'binary:logistic',
    'eval_metric': 'logloss',
    'max_depth': 4,
    'learning_rate': 0.05
    
}

model = xgb.train(params, dtrain, num_boost_round=100)

preds = model.predict(dtest)
binary_preds = [1 if p > 0.5 else 0 for p in preds]
print(f"accuracy: {accuracy_score(y_test, binary_preds) * 100:.2f}%")
print(f"roc-auc: {roc_auc_score(y_test, preds):.3f}")

model.save_model("model.json")