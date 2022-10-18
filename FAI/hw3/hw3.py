from argparse import ArgumentParser
from typing import Tuple, Union, List, Any
import numpy as np
import pandas as pd
import copy

from sklearn.linear_model import LogisticRegression

def data_preprocessing(data: pd.DataFrame) -> Tuple[pd.DataFrame, pd.DataFrame, pd.DataFrame, pd.DataFrame]:
    """
    Return the preprocessed data (X_train, X_test, y_train, y_test). 
    You will need to remove the "Email No." column since it is not a valid feature.
    """
    
    """
    It means input is data and its type is pd.DataFrame, output is a tuple
    """
    #200 rows, 80% training data, 20% test data
    #0 -> -1, 1 remains 1
    #Y -> prediction
    new_data = data.drop(["Email No."], axis=1)
    Y = data[["Prediction"]]
    #Y = data["Prediction"]#new
    y_train = Y.head(160)
    y_test = Y.tail(40)
    
    X = new_data.drop(["Prediction"], axis=1)
    x_train = X.head(160)
    x_test = X.tail(40)

    return (x_train, x_test, y_train, y_test)

class DecisionTree:
    "Add more of your code here if you want to"
    def __init__(self): 
        self.tree = {}
        self.predict_list = []
    def count_positive1(self, y: pd.DataFrame) -> int:
        a = 0 
        for i in range(len(y)):
            if y.iloc[i, 0] == 1:
                a+=1
        return a

    def count_negative1(self, y: pd.DataFrame) -> int:
        a = 0 
        for i in range(len(y)):
            if y.iloc[i ,0] == -1:
                a+=1
        return a

    def another_fit(self, X: pd.DataFrame, y: pd.DataFrame, prefix) -> None:
        "Fit the model with training data"
        min_confusion = 3 #need to change? 設成3夠不夠 是放在fit裡面嗎?
        number_of_pos1 = self.count_positive1(y) #因為是class的attribute，所以每次call一定要用self
        number_of_neg1 = self.count_negative1(y)
        if number_of_pos1 == 0 or number_of_neg1 == 0:
            if number_of_pos1 == 0:
                self.tree[prefix] = [-1] 
            else:
                self.tree[prefix] = [1]
            #return a leaf node use a dict to store it or store the whole tree
        else:
            concat_two = pd.concat([X, y], axis = 1) #可以在fit concat 這邊就直接輸入一個dataframe
            total_row = number_of_pos1 + number_of_neg1
            for i in range(len(concat_two.columns)): # 500 features 每個都要做一樣的事情
                left_pos = 0; left_neg = 0; cnt = 0;
                if(i == len(concat_two.columns) - 1):
                    break
                sort_concat = concat_two.sort_values(concat_two.columns[i], ascending=True) #Y也要跟著去sort
                prev = 0
                for j in range(len(sort_concat.index)):
                    cnt += 1
                    if(sort_concat.iloc[prev, i] != sort_concat.iloc[j, i]):
                        confusion_left = 2 * left_pos * left_neg / ((number_of_neg1 + number_of_pos1) * (left_pos + left_neg))
                        confusion_right = 2 * (number_of_pos1 - left_pos) * (number_of_neg1 - left_neg) / ((number_of_neg1 + number_of_pos1) * (number_of_pos1 - left_pos + number_of_neg1- left_neg))
                        total_confusion = confusion_left + confusion_right
                        if total_confusion < min_confusion:
                            min_confusion = total_confusion # store it
                            feature = i
                            cut = cnt - 1
                            threshold = (sort_concat.iloc[prev, i] + sort_concat.iloc[j, i]) / 2

                    if(sort_concat.iloc[j, len(sort_concat.columns) - 1] == 1): #update pos and neg
                        left_pos+=1
                    else:
                        left_neg+=1

                    prev = j

            self.tree[prefix] = [feature, threshold]

            concat_two = concat_two.sort_values(concat_two.columns[feature], ascending=True) #Y也要跟著去sort
            new_y = concat_two[["Prediction"]] # necessary?
            new_x = concat_two.drop(["Prediction"], axis=1)
            x_left = new_x.head(cut)
            y_left = new_y.head(cut)
            x_right = new_x.tail(total_row - cut)
            y_right = new_y.tail(total_row - cut)

            self.another_fit(x_left, y_left, prefix + "L")
            self.another_fit(x_right, y_right, prefix + "R")

    def fit(self, X: pd.DataFrame, y: pd.DataFrame) -> None:
        self.tree = {}
        self.another_fit(X, y, "S")

    def predict(self, X: pd.DataFrame) -> Any:
        self.predict_list = []
        "Make predictions for the testing data"
        for i in range(len(X.index)):
            curr_node = "S"
            curr_result = self.tree[curr_node]
            while(len(curr_result) != 1):
                if(X.iloc[i, curr_result[0]] >= curr_result[1]):
                    curr_node += "R"
                else:
                    curr_node += "L"
                curr_result = self.tree[curr_node]

            self.predict_list.append(curr_result[0])

        return self.predict_list

class RandomForest:
    "Add more of your code here if you want to"
    def __init__(self, seed: int = 42, num_trees: int = 5):
        self.num_trees = num_trees
        np.random.seed(seed)
        self.decision_tree_list = []
        self.predict_list = []

    def bagging(self, X: pd.DataFrame, y: pd.DataFrame) -> Tuple[pd.DataFrame, pd.DataFrame]:
        "DO NOT modify this function. This function is deliberately given to make your result reproducible."
        index = np.random.randint(0, X.shape[0], int(X.shape[0] / 2))
        return X.iloc[index, :], y.iloc[index]

    def fit(self, X: pd.DataFrame, y: pd.DataFrame) -> None:
        self.decision_tree_list = []
        self.predict_list = []
        for i in range(0, self.num_trees):
            x_new, y_new = self.bagging(X, y)
            dec_tree = DecisionTree()
            dec_tree.fit(x_new, y_new)
            #print("i=",i)
            dec_tree_new = copy.deepcopy(dec_tree.tree)
            self.decision_tree_list.append(dec_tree_new)
            #print("i=",i)

    def predict(self, X) -> Any:
        self.predict_list = []
        for j in range(len(X.index)):
            sum = 0
            for i in range(0, self.num_trees):
                curr_node = "S"
                curr_result = self.decision_tree_list[i][curr_node]
                while(len(curr_result) != 1):
                    if(X.iloc[j, curr_result[0]] >= curr_result[1]):
                        curr_node += "R"
                    else:
                        curr_node += "L"
                    curr_result = self.decision_tree_list[i][curr_node]

                sum += curr_result[0]
            
            #print(sum)
            if sum > 0:
                self.predict_list.append(1)
            else:
                self.predict_list.append(-1)

        return self.predict_list

def accuracy_score(y_pred: Any, y_label: Any) -> float:
    """
    y_pred: (1d array-like) your prediction
    y_label: (1d array-like) the groundtruth label
    Return the accuracy score
    """

    if(type(y_label) == pd.core.frame.DataFrame):
        y_label = y_label.values.squeeze()

    if(type(y_pred) == pd.core.frame.DataFrame):
        y_pred = y_pred.values.squeeze()

    TP = 0; FP = 0; TN = 0; FN = 0
    #print("length of y_pred=", len(y_pred))
    #print("length of y_label=", len(y_label))

    for i in range(0, len(y_pred)):
        if y_label[i] == 1 and y_pred[i] == 1:
            TP += 1
        if y_label[i] == -1 and y_pred[i] == 1:
            FP += 1
        if y_label[i] == -1 and y_pred[i] == -1:
            TN += 1
        if y_label[i] == 1 and y_pred[i] == -1:
            FN += 1

    if TP+TN+FP+FN==0:
        return 0
    else:
        return (TP + TN) / (TP + TN + FP + FN)

def f1_score(y_pred: Any, y_label: Any) -> float:
    """
    y_pred: (1d array-like) your prediction
    y_label: (1d array-like) the groundtruth label
    Return the F1 score
    """
    if(type(y_label) == pd.core.frame.DataFrame):
        y_label = y_label.values.squeeze()

    if(type(y_pred) == pd.core.frame.DataFrame):
        y_pred = y_pred.values.squeeze()

    TP = 0; FP = 0; TN = 0; FN = 0

    for i in range(0, len(y_pred)):
        if y_label[i] == 1 and y_pred[i] == 1:
            TP += 1
        if y_label[i] == -1 and y_pred[i] == 1:
            FP += 1
        if y_label[i] == -1 and y_pred[i] == -1:
            TN += 1
        if y_label[i] == 1 and y_pred[i] == -1:
            FN += 1

    if TP + FP == 0:
        Precision = 0
    else:
        Precision = TP / (TP + FP)

    if TP + FN == 0:
        Recall = 0
    else:
        Recall = TP / (TP + FN)

    if Precision + Recall == 0:
        F1 = 0
    else:
        F1 = 2 * (Precision * Recall) / (Precision + Recall)

    return F1

def cross_validation(model: Union[LogisticRegression, DecisionTree, RandomForest], X: pd.DataFrame, y: pd.DataFrame, folds: int = 5) -> Tuple[float, float]:
    """
    Test the generalizability of the model with 5-fold cross validation
    Return the mean accuracy and F1 score
    """###這有問題
    start = 0 ; accurary_sum = 0 ; F1_sum = 0; x_list = [] ; y_list = [] ;
    length = len(X.index)

    each_length = length // folds

    for i in range(0, folds):
        x_list.append(X[start:start + each_length])
        y_list.append(y[start:start + each_length])
        start += each_length
    
    for i in range(0, folds):
        new_x = pd.DataFrame()
        new_y = pd.DataFrame()
        predict_x = x_list[i]
        prediction_y = y_list[i]
        predict_y = pd.DataFrame()
        #predict_y = []
        #for l in range(len(prediction_y)):
            #predict_y =.append(prediction_y.iloc[l, 0])
        
        predict_y = pd.concat([predict_y, prediction_y])

        for j in range(0, folds):
            if i == j:
                continue

            new_x = pd.concat([new_x,x_list[j]])
            new_y = pd.concat([new_y,y_list[j]])
            #print("new_y=,",new_y)

        model.fit(new_x, new_y) # it has problem
        prediction_list = model.predict(predict_x)

        accurary_sum += accuracy_score(prediction_list, predict_y)
        F1_sum += f1_score(prediction_list, predict_y)

    mean_accuracy = accurary_sum / folds
    mean_F1 = F1_sum / folds
    return (mean_accuracy, mean_F1)

def tune_random_forest(choices: List[int], X: pd.DataFrame, y: pd.DataFrame) -> int:
    """
    choices: List of candidates for the number of decision trees in the random forest
    Return the best choice
    """
    best_f1, best_model = -1, None 

    for i in choices:
        model = RandomForest(num_trees = i)
        accuracy, f1 = cross_validation(model, X, y, 5)
        print(accuracy, f1)
        if f1 > best_f1:
            best_f1, best_model, best = f1, model, i
    
    print("best_f1 = ",best_f1)
    return best


def main(args):
    """
    This function is provided as a head start
    TA will use his own main function at test time.
    """
    data = pd.read_csv(args.data_path)
    print(data.head())
    print(data['Prediction'].value_counts())
    X_train, X_test, y_train, y_test = data_preprocessing(data)

    logistic_regression = LogisticRegression(solver='liblinear', max_iter=500)
    decision_tree = DecisionTree()
    #decision_tree.fit(X_train, y_train)  # the code I add
    #print(decision_tree.predict(X_test)) # the code I add
    #print(decision_tree.tree) # the code I add
    #exit(0) # the code I add 
    #"now decision tree is correct"
    random_forest = RandomForest()
    #random_forest.fit(X_train, y_train) # the code I add
    #print("test") # the code I add
    #print(random_forest.predict(X_test)) # the code I add
    #for i in range(len(random_forest.decision_tree_list)):
    #    print(random_forest.decision_tree_list[i])
    #exit(0) # the code I add
    
    models = [logistic_regression, decision_tree, random_forest]

    best_f1, best_model = -1, None
    for model in models:
        accuracy, f1 = cross_validation(model, X_train, y_train, 5)
        print(accuracy, f1)
        if f1 > best_f1:
            best_f1, best_model = f1, model
    
    best_model.fit(X_train, y_train)
    y_pred = best_model.predict(X_test)
    print(accuracy_score(y_pred, y_test), f1_score(y_pred, y_test))
    print(tune_random_forest([5, 11, 17], X_train, y_train))

def parse_arguments():
    parser = ArgumentParser()
    parser.add_argument('--data_path', type=str, default='./emails.csv')
    args = parser.parse_args()
    return args

if __name__ == '__main__':
    main(parse_arguments())
