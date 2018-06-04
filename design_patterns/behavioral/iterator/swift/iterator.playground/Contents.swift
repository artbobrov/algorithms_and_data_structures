import Foundation


struct Article {
	let text: String
}

struct Magazine {
	let articles: [Article]
}


struct MagazineIterator: IteratorProtocol {
	typealias Element = Article

	private var current = 0
	private let articles: [Article]


	init(articles: [Article]) {
		self.articles = articles
	}

	mutating func next() -> Article? {
		defer { current += 1 }
		return current < articles.count ? articles[current] : nil
	}
}

extension Magazine: Sequence {
	typealias Iterator = MagazineIterator

	func makeIterator() -> Magazine.Iterator {
		return MagazineIterator(articles: articles)
	}
}


let magazine = Magazine(articles: [
	Article(text: "Politics"),
	Article(text: "Sport"),
	Article(text: "Fashion"),
	Article(text: "Health")
])


for article in magazine {
	print("Article: \(article.text)")
}
