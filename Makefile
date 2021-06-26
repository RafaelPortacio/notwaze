.PHONY: serve
serve: src/backend/waze-server
	$(MAKE) serve-backend & $(MAKE) serve-frontend

.PHONY: cli
cli: src/backend/cli
	./$<

.PHONY: serve-backend
serve-backend: src/backend/waze-server rj_graph_database.json
	./$<

.PHONY: serve-frontend
serve-frontend:
	$(MAKE) -C src/frontend/ serve

.PHONY: test-shortest-path
test-shortest-path:
	$(MAKE) -C src/backend/ test

.PHONY: src/backend/waze-server
src/backend/waze-server:
	$(MAKE) -C src/backend/ waze-server

.PHONY: src/backend/cli
src/backend/cli:
	$(MAKE) -C src/backend/ cli

rj_graph_database.json:
	python3 src/create_data/create_graph_database.py
	sed -i 's/ //g' $@
