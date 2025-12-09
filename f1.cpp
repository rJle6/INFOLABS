template<typename ForwardIterator, typename UnaryPredicate>
ForwardIterator max_element_if(
    ForwardIterator first, ForwardIterator last, UnaryPredicate pred)
{
    ForwardIterator max_element_it = last;
    bool found = false;
    
    for (auto it = first; it != last; ++it) {
        if (pred(*it)) {
            max_element_it = it;
            found = true;
            break;
        }
    }
    
    if (!found) {
        return last;
    }
    
    for (auto it = max_element_it; it != last; ++it) {
        if (pred(*it) && *max_element_it < *it) {
            max_element_it = it;
        }
    }
    
    return max_element_it;
}






template<typename ForwardIterator, typename UnaryFunction>
void transform_inplace(ForwardIterator first, ForwardIterator last, UnaryFunction func)
{
    for (auto it = first; it != last; ++it) {
        func(*it);
    }
}