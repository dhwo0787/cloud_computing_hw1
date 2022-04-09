let inputBtn = document.getElementById("input-btn")

let myLeads = []
const inputEl = document.getElementById("input-el")
const ulEl = document.getElementById("ul-el")
const deleteBtn = document.getElementById("delete-btn")
const tabBtn = document.getElementById("tab-btn")
const cntBtn = document.getElementById("cnt-btn")
let cnt = 0;
//각자의 브라우저에서 사용되는 것
let leadsFromLocalStorage = JSON.parse(localStorage.getItem("myLeads"))

if(leadsFromLocalStorage) {
    myLeads = leadsFromLocalStorage
    renderLeads()
}

inputBtn.addEventListener("click", function() {
    myLeads.push(inputEl.value)
    inputEl.value = ""
    localStorage.setItem("myLeads",JSON.stringify(myLeads))
    renderLeads()

    // 같은 방법 두가지.
    //ulEl.innerHTML += "<li>" + inputEl.value + "</li>"
    /*const li = document.createElement("li")
    li.textContent = inputEl.value
    ulEl.append(li) */
})
deleteBtn.addEventListener("dblclick",function() {
    myLeads = []
    inputEl.value = ""
    localStorage.removeItem("myLeads")
    renderLeads()
  })

tabBtn.addEventListener("click",function(){
    chrome.tabs.query({active:true,currentWindow:true},function(tabs){
        let activeTab = tabs[0]
        let activeTabId = activeTab.id
        myLeads.push(tabs[0].url)
        localStorage.setItem("myLeads",JSON.stringify(myLeads))
        renderLeads()
    })
    
})
cntBtn.addEventListener("click", function() {
    cnt += 1;
    cntBtn.textContent = cnt;
})
function renderLeads() {
    let listItems = ""
    for(let i = 0; i<myLeads.length;i++) {
        //listItems += "<li><a target= '_blank' href='"+ myLeads[i] + "'>" + myLeads[i] + "</a></li>"
        listItems += `<li> <a target= '_blank' href= '${myLeads[i]}'> ${myLeads[i]}</a> </li>`
    }
    ulEl.innerHTML = listItems
}